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
    
    // Transform simplified if statements (before optional parens)
    result = this.transformSimplifiedIf(result);
    
    // Transform optional parentheses function calls
    result = this.transformOptionalParens(result);
    
    // Transform optional return statements (must be last)
    result = this.transformOptionalReturn(result);

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
    // Handle both multiline and single line with or without semicolons
    let result = source.replace(/^let\s+(\w+)\s*=\s*(.+)$/gm, 'const $1 = $2;');
    
    // Also handle inline let statements (not at start of line)
    result = result.replace(/\blet\s+(\w+)\s*=\s*([^;]+);?\s*/g, 'const $1 = $2; ');
    
    return result;
  }

  private transformPipeExpressions(source: string): string {
    // Transform pipe chains: value |> func1 |> func2 -> func2(func1(value))
    let result = source;
    
    // Find all pipe expressions and process them (but respect statement boundaries)
    result = result.replace(/([^;|\n]+(?:\|>[^;|\n]+)+)/g, (match) => {
      const parts = match.split('|>').map(part => part.trim());
      const value = parts[0];
      const functions = parts.slice(1);
      
      // Build nested function calls from right to left
      let expr = value;
      for (const func of functions) {
        if (func.includes('.')) {
          // Handle dot notation like console.log
          expr = `${func}(${expr})`;
        } else {
          // Handle method calls by converting to dot notation
          expr = `(${expr}).${func}()`;
        }
      }
      
      return expr;
    });
    
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
        
        for (let i = 0; i < caseLines.length; i++) {
          const caseLine = caseLines[i];
          const [pattern, action] = caseLine.split('=>').map(s => s.trim());
          
          if (pattern === '_') {
            if (i > 0) {
              result += `  } else {\n`;
            } else {
              result += `  {\n`;
            }
            result += `    return ${action};\n`;
          } else if (pattern.startsWith(':')) {
            const symbol = pattern.slice(1);
            if (i > 0) {
              result += `  } else if (__match_value === Symbol.for("${symbol}")) {\n`;
            } else {
              result += `  if (__match_value === Symbol.for("${symbol}")) {\n`;
            }
            result += `    return ${action};\n`;
          }
        }
        
        result += '  }\n})()';
        return result;
      }
    );
  }

  private transformOptionalParens(source: string): string {
    // Transform function calls without parentheses: myFunction x -> myFunction(x)
    // But skip strings and other contexts where it shouldn't apply
    
    // Split on strings to avoid processing inside them
    const parts = source.split(/("[^"]*"|'[^']*')/);
    
    for (let i = 0; i < parts.length; i += 2) { // Only process non-string parts
      if (parts[i]) {
        parts[i] = parts[i].replace(
          /\b(\w+)\s+([a-zA-Z_$][\w$]*|\d+|\[[^\]]*\]|\{[^}]*\})(?=\s|$|;|\)|,)/g,
          (match, funcName, arg) => {
            // Don't transform known keywords or operators
            const keywords = ['if', 'else', 'for', 'while', 'let', 'const', 'var', 'return', 'match', 'struct', 'trait', 'type', 'interface'];
            if (keywords.includes(funcName)) {
              return match;
            }
            return `${funcName}(${arg})`;
          }
        );
      }
    }
    
    // Now handle string arguments (which are preserved in the parts array)
    let result = parts.join('');
    
    // Process function calls with string arguments that were split out
    result = result.replace(
      /\b(\w+)\s+("[^"]*"|'[^']*')(?=\s|$|;|\)|,)/g,
      (match, funcName, stringArg) => {
        const keywords = ['if', 'else', 'for', 'while', 'let', 'const', 'var', 'return', 'match', 'struct', 'trait', 'type', 'interface'];
        if (keywords.includes(funcName)) {
          return match;
        }
        return `${funcName}(${stringArg})`;
      }
    );
    
    return result;
  }

  private transformSimplifiedIf(source: string): string {
    // Transform simplified if statements: if x == 1 { -> if (x == 1) {
    // But avoid matching "return if" or other contexts
    return source.replace(
      /(?:^|\s)(if\s+)([^{]+)\s*\{/g,
      (match, ifKeyword, condition) => {
        const trimmedCondition = condition.trim();
        // Only add parentheses if they're not already there
        if (!trimmedCondition.startsWith('(') || !trimmedCondition.endsWith(')')) {
          const leadingSpace = match.startsWith(' ') ? ' ' : '';
          return `${leadingSpace}${ifKeyword}(${trimmedCondition}) {`;
        }
        return match;
      }
    );
  }

  private transformOptionalReturn(source: string): string {
    // Transform functions to auto-return last expression
    // Match function declarations and arrow functions
    return source.replace(
      /(function\s+\w+\([^)]*\)\s*\{|\([^)]*\)\s*=>\s*\{|=>\s*\{)([^}]+)\}/g,
      (match, funcStart, body) => {
        const lines = body.trim().split(';').filter(line => line.trim());
        if (lines.length === 0) return match;
        
        const lastLine = lines[lines.length - 1].trim();
        const otherLines = lines.slice(0, -1);
        
        // Don't add return if it already has one or if it's a control flow statement
        if (lastLine.startsWith('return ') || lastLine.startsWith('if ') || lastLine.includes('if (')) {
          return match;
        }
        
        // Build the new function body
        let newBody = '';
        if (otherLines.length > 0) {
          newBody = otherLines.join(';') + '; ';
        }
        newBody += `return ${lastLine}`;
        
        return `${funcStart} ${newBody} }`;
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