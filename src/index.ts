#!/usr/bin/env bun

import { spawn } from "bun";
import { join } from "path";
import { ZenoscriptTranspiler } from "./transpiler.ts";

async function runTranspiler(args: string[]) {
  // Parse arguments
  const verbose = args.includes('--verbose') || args.includes('-V');
  const debug = args.includes('--debug') || args.includes('-d');
  
  const fileArgs = args.filter(arg => !arg.startsWith('-'));
  const inputFile = fileArgs[0];
  const outputFile = fileArgs[1];

  if (!inputFile) {
    console.error('Error: No input file specified');
    console.error("Try 'zeno --help' for more information.");
    process.exit(1);
  }

  // Check if input file exists
  const inputExists = await Bun.file(inputFile).exists();
  if (!inputExists) {
    console.error(`Error: Input file '${inputFile}' not found`);
    process.exit(1);
  }

  const transpiler = new ZenoscriptTranspiler({ verbose, debug });
  
  try {
    const source = await Bun.file(inputFile).text();
    const result = transpiler.transpile(source);
    
    if (outputFile) {
      await Bun.write(outputFile, result);
      if (verbose) {
        console.log(`Output written to '${outputFile}'`);
      }
    } else {
      console.log(result);
    }
  } catch (error) {
    console.error('Transpilation failed:', error.message);
    process.exit(1);
  }
}

async function initProject() {
  console.log("Initializing Zenoscript project...");
  
  const packageJson = {
    name: "zenoscript-project",
    version: "1.0.0",
    description: "A Zenoscript project",
    type: "module",
    scripts: {
      dev: "bun --hot index.zs",
      build: "bun build index.zs --outdir dist",
      start: "bun dist/index.js"
    },
    devDependencies: {
      "@types/bun": "latest"
    },
    dependencies: {
      zenoscript: "latest"
    }
  };
  
  const bunfig = {
    preload: ["zenoscript/plugin"],
    plugins: ["zenoscript/plugin"]
  };
  
  const indexZs = `// Welcome to Zenoscript!
// This is a functional programming language that compiles to TypeScript

const greet = (name: string) => \`Hello, \${name}!\`;

const main = () => {
  console.log(greet("World"));
};

main();
`;

  try {
    await Bun.write("package.json", JSON.stringify(packageJson, null, 2));
    await Bun.write("bunfig.toml", `preload = ["zenoscript/plugin"]

[plugins]
zenoscript = "zenoscript/plugin"
`);
    await Bun.write("index.zs", indexZs);
    
    console.log("✓ Created package.json");
    console.log("✓ Created bunfig.toml");
    console.log("✓ Created index.zs");
    console.log("\nNext steps:");
    console.log("1. Run 'bun install' to install dependencies");
    console.log("2. Run 'bun dev' to start development");
    console.log("3. Run 'bun build' to build for production");
    
    // Auto-setup the plugin
    await setupPlugin();
  } catch (error) {
    console.error("Failed to initialize project:", error.message);
    process.exit(1);
  }
}

async function setupPlugin() {
  console.log("Setting up Zenoscript plugin for Bun...");
  
  try {
    const bunfigExists = await Bun.file("bunfig.toml").exists();
    const packageExists = await Bun.file("package.json").exists();
    
    if (!bunfigExists) {
      await Bun.write("bunfig.toml", `preload = ["zenoscript/plugin"]

[plugins]
zenoscript = "zenoscript/plugin"
`);
      console.log("✓ Created bunfig.toml with Zenoscript plugin");
    } else {
      const bunfig = await Bun.file("bunfig.toml").text();
      if (!bunfig.includes("zenoscript")) {
        const updated = bunfig + `\n[plugins]\nzenoscript = "zenoscript/plugin"\n`;
        await Bun.write("bunfig.toml", updated);
        console.log("✓ Updated bunfig.toml with Zenoscript plugin");
      } else {
        console.log("✓ Zenoscript plugin already configured in bunfig.toml");
      }
    }
    
    if (packageExists) {
      const pkg = await Bun.file("package.json").json();
      if (!pkg.dependencies?.zenoscript && !pkg.devDependencies?.zenoscript) {
        console.log("📦 Installing Zenoscript dependency...");
        const installProcess = spawn({
          cmd: ["bun", "add", "zenoscript"],
          stdio: ["inherit", "inherit", "inherit"],
        });
        await installProcess.exited;
      }
    }
    
    console.log("✓ Zenoscript plugin setup complete!");
  } catch (error) {
    console.error("Failed to setup plugin:", error.message);
    process.exit(1);
  }
}

async function executeInline(code: string) {
  const transpiler = new ZenoscriptTranspiler({ debug: false });
  
  try {
    // Transpile the input
    const typescript = transpiler.transpile(code);
    
    // Execute the TypeScript - wrap in an IIFE for multiple statements
    try {
      const wrappedCode = `(function() { ${typescript} })()`;
      const result = eval(wrappedCode);
      if (result !== undefined) {
        console.log(result);
      }
    } catch (execError) {
      console.error("Execution error:", execError.message);
      process.exit(1);
    }
  } catch (error) {
    console.error("Transpilation error:", error.message);
    process.exit(1);
  }
}

async function startRepl() {
  console.log("Starting Zenoscript REPL...");
  console.log("Type 'exit' or press Ctrl+C to quit\n");
  
  const transpiler = new ZenoscriptTranspiler({ debug: false });
  
  const readline = require("readline");
  const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    prompt: "zs> "
  });
  
  rl.prompt();
  
  rl.on("line", async (input) => {
    const trimmed = input.trim();
    
    if (trimmed === "exit" || trimmed === "quit") {
      rl.close();
      return;
    }
    
    if (trimmed === "") {
      rl.prompt();
      return;
    }
    
    try {
      // Transpile the input
      const typescript = transpiler.transpile(trimmed);
      console.log("→", typescript.trim());
      
      // Execute the TypeScript
      try {
        const result = eval(typescript);
        if (result !== undefined) {
          console.log(result);
        }
      } catch (execError) {
        console.error("Execution error:", execError.message);
      }
    } catch (error) {
      console.error("REPL error:", error.message);
    }
    
    rl.prompt();
  });
  
  rl.on("close", () => {
    console.log("\nGoodbye!");
    process.exit(0);
  });
}

async function main() {
  const args = process.argv.slice(2);
  const command = args[0];
  
  // Handle -e flag for inline execution
  const evalIndex = args.indexOf('-e');
  if (evalIndex !== -1) {
    if (evalIndex + 1 >= args.length) {
      console.error('Error: No code provided after -e flag');
      process.exit(1);
    }
    const code = args[evalIndex + 1];
    await executeInline(code);
    return;
  }
  
  // Handle special commands
  if (args.length === 0 || args.includes("--help") || args.includes("-h")) {
    console.log(`Zenoscript CLI

Usage: zeno [command] [options] [file]

Commands:
  init                    Initialize a new Zenoscript project
  setup                   Setup Zenoscript plugin for Bun
  repl                    Start interactive REPL
  -e <code>               Execute Zenoscript code inline
  <file>                  Transpile a Zenoscript file

Options:
  --help, -h             Show help
  --version, -v          Show version
  --verbose, -V          Verbose output
  --debug, -d            Debug output

Examples:
  zeno init              # Initialize new project
  zeno setup             # Setup plugin
  zeno repl              # Start REPL
  zeno -e '"Hello" |> console.log'  # Execute code inline
  zeno input.zs          # Transpile file
  zeno input.zs output.ts # Transpile to specific output
`);
    return;
  }
  
  if (args.includes("--version") || args.includes("-v")) {
    let version = "0.1.3"; // fallback version
    
    // Try to read VERSION file from different possible locations
    const possiblePaths = [
      join(import.meta.dir, "..", "VERSION"),  // development
      join(process.cwd(), "VERSION"),          // current directory
      "VERSION",                               // relative to current directory
    ];
    
    for (const path of possiblePaths) {
      try {
        const file = Bun.file(path);
        if (await file.exists()) {
          version = (await file.text()).trim();
          break;
        }
      } catch (error) {
        // Continue to next path
      }
    }
    
    console.log(`Zenoscript v${version}`);
    return;
  }
  
  // Handle subcommands
  switch (command) {
    case "init":
      await initProject();
      break;
    case "setup":
      await setupPlugin();
      break;
    case "repl":
      await startRepl();
      break;
    default:
      // Pass all arguments to the transpiler
      await runTranspiler(args);
      break;
  }
}

// Handle uncaught errors
process.on("uncaughtException", (error) => {
  console.error("Zenoscript CLI error:", error.message);
  process.exit(1);
});

process.on("unhandledRejection", (reason) => {
  console.error("Zenoscript CLI error:", reason);
  process.exit(1);
});

main().catch((error) => {
  console.error("Zenoscript CLI error:", error.message);
  process.exit(1);
});