#!/usr/bin/env bun

import { spawn } from "bun";
import { join } from "path";

const TRANSPILER_DIR = join(import.meta.dir, "transpiler");
const BUILD_DIR = join(import.meta.dir, "..", "build");
const TRANSPILER_BINARY = join(BUILD_DIR, "zeno");

async function buildTranspiler() {
  console.log("Building Zenoscript transpiler with tinycc...");
  
  const makeProcess = spawn({
    cmd: ["make"],
    cwd: TRANSPILER_DIR,
    stdio: ["inherit", "inherit", "inherit"],
  });
  
  const exitCode = await makeProcess.exited;
  if (exitCode !== 0) {
    console.error("Failed to build transpiler");
    process.exit(1);
  }
  
  console.log("Transpiler built successfully!");
}

async function runTranspiler(args: string[]) {
  // Check if transpiler binary exists
  const binaryExists = await Bun.file(TRANSPILER_BINARY).exists();
  
  if (!binaryExists) {
    await buildTranspiler();
  }
  
  // Run the transpiler with the provided arguments
  const transpilerProcess = spawn({
    cmd: [TRANSPILER_BINARY, ...args],
    stdio: ["inherit", "inherit", "inherit"],
  });
  
  const exitCode = await transpilerProcess.exited;
  process.exit(exitCode);
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

async function startRepl() {
  console.log("Starting Zenoscript REPL...");
  console.log("Type 'exit' or press Ctrl+C to quit\n");
  
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
      // Create a temporary file with the input
      const tempFile = `/tmp/zenoscript_repl_${Date.now()}.zs`;
      await Bun.write(tempFile, trimmed);
      
      // Transpile and execute
      const transpilerProcess = spawn({
        cmd: [TRANSPILER_BINARY, tempFile],
        stdout: "pipe",
        stderr: "pipe",
      });
      
      const exitCode = await transpilerProcess.exited;
      
      if (exitCode === 0) {
        const typescript = await new Response(transpilerProcess.stdout).text();
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
      } else {
        const stderr = await new Response(transpilerProcess.stderr).text();
        console.error("Error:", stderr);
      }
      
      // Cleanup
      await Bun.$`rm -f ${tempFile}`;
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
  
  // Handle special commands
  if (args.length === 0 || args.includes("--help") || args.includes("-h")) {
    console.log(`Zenoscript CLI

Usage: zeno [command] [options] [file]

Commands:
  init                    Initialize a new Zenoscript project
  setup                   Setup Zenoscript plugin for Bun
  repl                    Start interactive REPL
  <file>                  Transpile a Zenoscript file

Options:
  --help, -h             Show help
  --version, -v          Show version
  --verbose, -V          Verbose output
  --debug, -d            Debug output
  --build                Build transpiler from source

Examples:
  zeno init              # Initialize new project
  zeno setup             # Setup plugin
  zeno repl              # Start REPL
  zeno input.zs          # Transpile file
  zeno input.zs output.ts # Transpile to specific output
`);
    return;
  }
  
  if (args.includes("--version") || args.includes("-v")) {
    const version = await Bun.file(join(import.meta.dir, "..", "VERSION")).text();
    console.log(`Zenoscript v${version.trim()}`);
    return;
  }
  
  if (args.includes("--build")) {
    await buildTranspiler();
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
      // Check if transpiler binary exists
      const binaryExists = await Bun.file(TRANSPILER_BINARY).exists();
      if (!binaryExists) {
        await buildTranspiler();
      }
      
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