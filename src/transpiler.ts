#!/usr/bin/env bun

// Simple TypeScript-based Zenoscript transpiler
// This replaces the C binary with a more maintainable TypeScript implementation

export interface TranspilerOptions {
  verbose?: boolean;
  debug?: boolean;
}

export class ZenoscriptTranspiler {
  private options: TranspilerOptions;

  constructor(options: TranspilerOptions = {}) {
    this.options = options;
  }

  transpile(source: string): string {
    if (this.options.debug) {
      console.log("=== Source ===");
      console.log(source);
      console.log("\n=== Generated TypeScript ===");
    }

    let result = source;

    // Basic transformations - simplified for now
    // These would be expanded to handle the full language
    
    // Transform struct declarations to TypeScript types
    result = this.transformStructs(result);
    
    // Transform trait declarations to TypeScript interfaces
    result = this.transformTraits(result);
    
    // Transform let bindings to const declarations
    result = this.transformLetBindings(result);
    
    // Transform pipe expressions
    result = this.transformPipeExpressions(result);
    
    // Transform match expressions (before atoms to preserve pattern matching)
    result = this.transformMatchExpressions(result);
    
    // Transform atoms to symbols
    result = this.transformAtoms(result);

    if (this.options.verbose) {
      console.log("Transpilation completed successfully");
    }

    return result;
  }

  async transpileFile(inputPath: string, outputPath?: string): Promise<string> {
    if (this.options.verbose) {
      console.log(`Transpiling '${inputPath}'...`);
    }

    const source = await Bun.file(inputPath).text();
    const result = this.transpile(source);

    if (outputPath) {
      await Bun.write(outputPath, result);
      if (this.options.verbose) {
        console.log(`Output written to '${outputPath}'`);
      }
    }

    return result;
  }

  private transformStructs(source: string): string {
    // Transform: struct Name { field: type; } -> type Name = { field: type; };
    let result = source.replace(
      /struct\s+(\w+)(?:<([^>]+)>)?\s*\{([^}]*)\}/g,
      (match, name, generics, body) => {
        const genericPart = generics ? `<${generics}>` : '';
        return `type ${name}${genericPart} = {${body}};`;
      }
    );
    
    // Handle empty structs: struct Name; -> type Name = {};
    result = result.replace(/struct\s+(\w+);/g, 'type $1 = {};');
    
    return result;
  }

  private transformTraits(source: string): string {
    // Transform: trait Name { method(): type; } -> interface Name { method(): type; }
    return source.replace(/trait\s+(\w+)(?:<([^>]+)>)?\s*\{/g, 'interface $1$2 {');
  }

  private transformLetBindings(source: string): string {
    // Transform: let name = value -> const name = value;
    return source.replace(/^let\s+(\w+)\s*=\s*(.+)$/gm, 'const $1 = $2;');
  }

  private transformPipeExpressions(source: string): string {
    // Transform: value |> func -> value.func()
    // Handle chained pipes: value |> func1 |> func2 -> value.func1().func2()
    let result = source;
    
    // Handle quoted strings and function calls in pipes
    result = result.replace(/("[^"]*")\s*\|\>\s*(\w+)/g, '$1.$2()');
    result = result.replace(/(\w+(?:\.\w+)*(?:\(\))?)\s*\|\>\s*(\w+)/g, '$1.$2()');
    
    return result;
  }

  private transformAtoms(source: string): string {
    // Transform: :atom -> Symbol.for("atom")
    return source.replace(/:(\w+)/g, 'Symbol.for("$1")');
  }

  private transformMatchExpressions(source: string): string {
    // Transform match expressions to if-else chains
    return source.replace(
      /match\s+(\w+)\s*\{([^}]+)\}/gs,
      (match, variable, cases) => {
        // Split cases by newlines and filter out empty lines
        const caseLines = cases.trim().split('\n')
          .map(line => line.trim())
          .filter(line => line && line.includes('=>'));
        
        let result = '(() => {\n';
        result += `  const __match_value = ${variable};\n`;
        
        let hasConditions = false;
        
        for (let i = 0; i < caseLines.length; i++) {
          const caseLine = caseLines[i];
          const [pattern, action] = caseLine.split('=>').map(s => s.trim());
          
          if (pattern === '_') {
            if (hasConditions) {
              result += `  } else {\n`;
            } else {
              result += `  if (true) {\n`;
            }
            result += `    return ${action};\n`;
          } else if (pattern.startsWith(':')) {
            const symbol = pattern.slice(1);
            const condition = hasConditions ? '} else if' : 'if';
            result += `  ${condition} (__match_value === Symbol.for("${symbol}")) {\n`;
            result += `    return ${action};\n`;
            hasConditions = true;
          }
        }
        
        result += '  }\n})()';
        return result;
      }
    );
  }
}

// CLI interface for when this is called directly
if (import.meta.main) {
  const args = process.argv.slice(2);
  
  if (args.includes('--help') || args.includes('-h')) {
    console.log(`Zenoscript Transpiler

Usage: bun transpiler.ts [options] <input-file> [output-file]

Options:
  -h, --help     Show this help
  -v, --verbose  Verbose output  
  -d, --debug    Debug output

Examples:
  bun transpiler.ts input.zs
  bun transpiler.ts input.zs output.ts
  bun transpiler.ts --debug input.zs`);
    process.exit(0);
  }

  const verbose = args.includes('--verbose') || args.includes('-V');
  const debug = args.includes('--debug') || args.includes('-d');
  
  const fileArgs = args.filter(arg => !arg.startsWith('-'));
  const inputFile = fileArgs[0];
  const outputFile = fileArgs[1];

  if (!inputFile) {
    console.error('Error: No input file specified');
    process.exit(1);
  }

  const transpiler = new ZenoscriptTranspiler({ verbose, debug });
  
  try {
    const result = await transpiler.transpileFile(inputFile, outputFile);
    
    if (!outputFile) {
      console.log(result);
    }
  } catch (error) {
    console.error('Transpilation failed:', error.message);
    process.exit(1);
  }
}