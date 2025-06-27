import { plugin } from "bun";
import zenoscriptPlugin from "./plugin";

// Auto-register the Zenoscript plugin
plugin(zenoscriptPlugin);