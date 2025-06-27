import { defineConfig } from 'vitepress'

// https://vitepress.vuejs.org/config/app-configs
export default defineConfig({
  title: 'Zenoscript',
  description: 'A fast, functional programming language that compiles to TypeScript',
  
  head: [
    ['link', { rel: 'icon', href: '/zenoscript_mascot.svg' }],
    ['link', { rel: 'shortcut icon', href: '/zenoscript_mascot.svg' }],
    ['link', { rel: 'apple-touch-icon', href: '/zenoscript_mascot.svg' }],
    ['meta', { name: 'theme-color', content: '#bd34fe' }],
    ['meta', { property: 'og:type', content: 'website' }],
    ['meta', { property: 'og:locale', content: 'en' }],
    ['meta', { property: 'og:title', content: 'Zenoscript | Fast functional programming for TypeScript' }],
    ['meta', { property: 'og:site_name', content: 'Zenoscript' }],
    ['meta', { property: 'og:image', content: 'https://zeno.run/zenoscript_mascot.svg' }],
    ['meta', { property: 'og:url', content: 'https://zeno.run/' }],
    ['meta', { property: 'twitter:card', content: 'summary_large_image' }],
    ['meta', { property: 'twitter:image', content: 'https://zeno.run/zenoscript_mascot.svg' }]
  ],

  themeConfig: {
    logo: '/zenoscript_mascot.svg',
    
    nav: [
      { text: 'Docs', link: '/docs/' },
      { text: 'Examples', link: '/examples/' },
      { text: 'Blog', link: '/blog/' },
      { 
        text: 'v0.1.3',
        items: [
          { text: 'Changelog', link: '/changelog' },
          { text: 'Contributing', link: '/contributing' }
        ]
      }
    ],

    sidebar: {
      '/docs/': [
        {
          text: 'Getting Started',
          items: [
            { text: 'Introduction', link: '/docs/' },
            { text: 'Installation', link: '/docs/installation' },
            { text: 'Quick Start', link: '/docs/quickstart' }
          ]
        },
        {
          text: 'Language Guide',
          items: [
            { text: 'Syntax', link: '/docs/syntax' },
            { text: 'Types', link: '/docs/types' },
            { text: 'Functions', link: '/docs/functions' },
            { text: 'Pattern Matching', link: '/docs/pattern-matching' },
            { text: 'Structs & Traits', link: '/docs/structs-traits' }
          ]
        },
        {
          text: 'Tools',
          items: [
            { text: 'CLI', link: '/docs/cli' },
            { text: 'REPL', link: '/docs/repl' },
            { text: 'Bun Integration', link: '/docs/bun' },
            { text: 'FAQ', link: '/docs/faq' }
          ]
        }
      ],
      '/examples/': [
        {
          text: 'Examples',
          items: [
            { text: 'Overview', link: '/examples/' },
            { text: 'Basic Usage', link: '/examples/basic' },
            { text: 'Advanced', link: '/examples/advanced' },
            { text: 'Web Development', link: '/examples/web' }
          ]
        }
      ],
      '/blog/': [
        {
          text: 'Blog',
          items: [
            { text: 'Introducing Zenoscript', link: '/blog/introducing-zenoscript' }
          ]
        }
      ]
    },

    socialLinks: [
      { icon: 'github', link: 'https://github.com/wess/zenoscript' }
    ],

    footer: {
      message: 'Released under the MIT License.',
      copyright: 'Copyright © 2024-present Zenoscript Team'
    },

    editLink: {
      pattern: 'https://github.com/wess/zenoscript/edit/main/site/docs/:path'
    },

    search: {
      provider: 'local'
    }
  },

  markdown: {
    theme: {
      light: 'github-light',
      dark: 'github-dark'
    },
    config(md) {
      // Enhance markdown-it to handle Zenoscript
      const originalRender = md.renderer.rules.fence || ((tokens, idx, options, env, renderer) => {
        return renderer.renderToken(tokens, idx, options)
      })
      
      md.renderer.rules.fence = (tokens, idx, options, env, renderer) => {
        const token = tokens[idx]
        const info = token.info ? token.info.trim() : ''
        
        // Map zenoscript to typescript for syntax highlighting
        if (info === 'zenoscript' || info === 'zs') {
          token.info = 'typescript'
        }
        
        // Map dockerfile to docker for syntax highlighting
        if (info === 'dockerfile') {
          token.info = 'docker'
        }
        
        return originalRender(tokens, idx, options, env, renderer)
      }
    }
  }
})
