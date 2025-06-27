import DefaultTheme from 'vitepress/theme'
import './style.css'
import { enhanceZenoscriptHighlighting } from './zenoscript-highlight.js'
import InstallTabs from './components/InstallTabs.vue'

export default {
  ...DefaultTheme,
  enhanceApp({ app, router, siteData }) {
    // Call the default theme's enhanceApp if it exists
    if (DefaultTheme.enhanceApp) {
      DefaultTheme.enhanceApp({ app, router, siteData })
    }
    
    // Register global components
    app.component('InstallTabs', InstallTabs)
    
    // Initialize Zenoscript syntax highlighting enhancement
    if (typeof window !== 'undefined') {
      enhanceZenoscriptHighlighting()
    }
  }
}