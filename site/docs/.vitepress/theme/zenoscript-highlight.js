// Custom Zenoscript syntax highlighting enhancement
// This runs after the page loads to enhance TypeScript highlighting for Zenoscript-specific syntax

export function enhanceZenoscriptHighlighting() {
  // Wait for page load
  if (typeof window !== 'undefined') {
    // Enhance highlighting after DOM is ready
    const observer = new MutationObserver((mutations) => {
      mutations.forEach((mutation) => {
        if (mutation.type === 'childList') {
          enhanceCodeBlocks();
        }
      });
    });

    // Start observing
    if (document.body) {
      observer.observe(document.body, {
        childList: true,
        subtree: true
      });
    }

    // Initial enhancement
    document.addEventListener('DOMContentLoaded', enhanceCodeBlocks);
  }
}

function enhanceCodeBlocks() {
  const codeBlocks = document.querySelectorAll('pre[class*="language-typescript"] code');
  
  codeBlocks.forEach(block => {
    let html = block.innerHTML;
    
    // Highlight atoms (:symbol)
    html = html.replace(
      /(\s|^|>)(:)([a-zA-Z_][a-zA-Z0-9_]*)\b/g,
      '$1<span class="token zenoscript-atom">$2$3</span>'
    );
    
    // Highlight pipe operators
    html = html.replace(
      /(\|&gt;)/g,
      '<span class="token operator pipe">$1</span>'
    );
    
    // Highlight match keyword
    html = html.replace(
      /\b(match|struct|trait|impl)\b/g,
      '<span class="token keyword zenoscript">$1</span>'
    );
    
    // Highlight underscore in pattern matching
    html = html.replace(
      /(\s|^|>)(_)(\s|$|&)/g,
      '$1<span class="token wildcard">$2</span>$3'
    );
    
    block.innerHTML = html;
  });
}

// Auto-run if in browser environment
if (typeof window !== 'undefined') {
  enhanceZenoscriptHighlighting();
}