import * as vscode from 'vscode';

export function activate(context: vscode.ExtensionContext) {
    console.log('Zenoscript extension is now active!');

    // Register a command for Zenoscript
    let disposable = vscode.commands.registerCommand('zenoscript.helloWorld', () => {
        vscode.window.showInformationMessage('Hello from Zenoscript!');
    });

    context.subscriptions.push(disposable);

    // Register language features
    const provider = vscode.languages.registerDocumentFormattingEditProvider('zenoscript', {
        provideDocumentFormattingEdits(document: vscode.TextDocument): vscode.TextEdit[] {
            // Basic formatting for Zenoscript
            const edits: vscode.TextEdit[] = [];
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

export function deactivate() {}