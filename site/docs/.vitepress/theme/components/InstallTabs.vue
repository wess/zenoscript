<template>
  <div class="install-tabs" :key="componentKey">
    <div class="tabs">
      <button 
        v-for="tab in tabs" 
        :key="`${componentKey}-${tab.id}`"
        :class="{ active: activeTab === tab.id }"
        @click="setActiveTab(tab.id)"
      >
        {{ tab.label }}
      </button>
    </div>
    <div class="tab-content" :key="`${componentKey}-content`">
      <div v-if="activeTab === 'curl'" class="tab-pane">
        <pre><code class="language-bash">curl -fsSL https://zeno.run/install.sh | bash</code></pre>
      </div>
      <div v-else-if="activeTab === 'brew'" class="tab-pane">
        <pre><code class="language-bash">brew install wess/packages/zenoscript</code></pre>
      </div>
      <div v-else-if="activeTab === 'windows'" class="tab-pane">
        <pre><code class="language-powershell">iwr https://zeno.run/install.ps1 | iex</code></pre>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref } from 'vue'

const activeTab = ref('curl')
const componentKey = ref(Math.random().toString(36).substr(2, 9))

const setActiveTab = (tabId) => {
  activeTab.value = tabId
}

const tabs = [
  { id: 'curl', label: 'macOS/Linux' },
  { id: 'brew', label: 'Homebrew' },
  { id: 'windows', label: 'Windows' }
]
</script>

<style scoped>
.install-tabs {
  border: 1px solid var(--vp-c-border);
  border-radius: 8px;
  overflow: hidden;
  margin: 16px 0;
}

.tabs {
  display: flex;
  background: var(--vp-c-bg-soft);
  border-bottom: 1px solid var(--vp-c-border);
}

.tabs button {
  flex: 1;
  padding: 12px 16px;
  border: none;
  background: transparent;
  color: var(--vp-c-text-2);
  cursor: pointer;
  transition: all 0.2s;
  font-size: 14px;
  font-weight: 500;
}

.tabs button:hover {
  color: var(--vp-c-text-1);
  background: var(--vp-c-bg-alt);
}

.tabs button.active {
  color: var(--vp-c-brand-1);
  background: var(--vp-c-bg);
  border-bottom: 2px solid var(--vp-c-brand-1);
}

.tab-content {
  padding: 0;
}

.tab-content pre {
  margin: 0;
  border-radius: 0;
  border: none;
}

.tab-content code {
  padding: 16px;
  display: block;
}

.tab-pane {
  display: block;
}

.tab-pane pre {
  margin: 0;
}
</style>