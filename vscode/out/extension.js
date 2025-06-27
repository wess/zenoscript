"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.deactivate = exports.activate = void 0;
const vscode = require("vscode");
function activate(context) {
    console.log('Zenoscript extension is now active!');
    // Register a command for Zenoscript
    let disposable = vscode.commands.registerCommand('zenoscript.helloWorld', () => {
        vscode.window.showInformationMessage('Hello from Zenoscript!');
    });
    context.subscriptions.push(disposable);
    // Register language features
    const provider = vscode.languages.registerDocumentFormattingEditProvider('zenoscript', {
        provideDocumentFormattingEdits(document) {
            // Basic formatting for Zenoscript
            const edits = [];
            for (let i = 0; i < document.lineCount; i++) {
                const line = document.lineAt(i);
                const trimmed = line.text.trim();
                if (trimmed !== line.text) {
                    edits.push(vscode.TextEdit.replace(line.range, trimmed));
                }
            }
            return edits;
        }
    });
    context.subscriptions.push(provider);
}
exports.activate = activate;
function deactivate() { }
exports.deactivate = deactivate;
//# sourceMappingURL=extension.js.map