import { plugin } from "bun";
import { spawn } from "bun";
import { join } from "node:path";
import { existsSync } from "node:fs";

const BUILD_DIR = join(import.meta.dir, "..", "build");
const TRANSPILER_BINARY = join(BUILD_DIR, "zeno");

async function ensureTranspilerBuilt() {
  if (!existsSync(TRANSPILER_BINARY)) {
    console.log("Building Zenoscript transpiler...");
    const makeProcess = spawn({
      cmd: ["make"],
      cwd: join(import.meta.dir, "transpiler"),
      stdio: ["inherit", "inherit", "inherit"],
    });

    const exitCode = await makeProcess.exited;
    if (exitCode !== 0) {
      throw new Error("Failed to build Zenoscript transpiler");
    }
  }
}

async function transpileZenoscript(source: string, _path: string): Promise<string> {
  await ensureTranspilerBuilt();

  // Write source to temp file
  const tempFile = `/tmp/zenoscript_${Date.now()}_${Math.random().toString(36).substr(2, 9)}.zs`;
  await Bun.write(tempFile, source);

  try {
    // Run transpiler
    const process = spawn({
      cmd: [TRANSPILER_BINARY, tempFile],
      stdout: "pipe",
      stderr: "pipe",
    });

    const exitCode = await process.exited;

    if (exitCode !== 0) {
      const stderr = await new Response(process.stderr).text();
      throw new Error(`Zenoscript transpilation failed: ${stderr}`);
    }

    const typescript = await new Response(process.stdout).text();
    return typescript;
  } finally {
    // Cleanup temp file
    await Bun.$`rm -f ${tempFile}`;
  }
}

export const zenoscriptPlugin = plugin({
  name: "zenoscript",
  setup(build) {
    // Handle .zs files for both bundling and loading
    build.onLoad({ filter: /\.zs$/ }, async (args) => {
      const source = await Bun.file(args.path).text();

      try {
        const typescript = await transpileZenoscript(source, args.path);

        return {
          contents: typescript,
          loader: "ts", // Treat output as TypeScript
        };
      } catch (error) {
        return {
          errors: [
            {
              text: `Zenoscript transpilation error: ${error.message}`,
              location: { file: args.path },
            },
          ],
        };
      }
    });

    // Handle .zs files as entry points
    build.onResolve({ filter: /\.zs$/ }, (args) => {
      return {
        path: args.path,
        namespace: "zenoscript",
      };
    });

    // Handle relative imports of .zs files
    build.onResolve({ filter: /^\..*\.zs$/ }, (args) => {
      const path = require("node:path");
      const resolvedPath = path.resolve(args.resolveDir, args.path);
      return {
        path: resolvedPath,
        namespace: "zenoscript",
      };
    });
  },
});

export default zenoscriptPlugin;
