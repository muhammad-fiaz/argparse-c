<template>
  <nav v-if="items.length > 1" class="breadcrumb" aria-label="Breadcrumb">
    <ol>
      <li v-for="(item, index) in items" :key="index">
        <span v-if="index === items.length - 1" class="current" aria-current="page">
          {{ item.text }}
        </span>
        <a v-else :href="item.link">{{ item.text }}</a>
        <span v-if="index < items.length - 1" class="separator" aria-hidden="true">/</span>
      </li>
    </ol>
  </nav>
</template>

<script setup>
import { computed } from 'vue'
import { useData } from 'vitepress'

const { page } = useData()

const items = computed(() => {
  const path = page.value.relativePath.replace(/\.md$/, '')
  const segments = path.split('/').filter(Boolean)
  const crumbs = [{ text: 'Home', link: '/' }]

  const sectionMap = {
    guide: 'Guide',
    api: 'API Reference',
    examples: 'Examples'
  }

  let currentPath = ''
  segments.forEach((segment, index) => {
    currentPath += '/' + segment
    const isLast = index === segments.length - 1

    if (segment === 'guide' || segment === 'api' || segment === 'examples') {
      crumbs.push({ text: sectionMap[segment], link: currentPath + '/' })
    } else if (!isLast) {
      crumbs.push({ text: segment.charAt(0).toUpperCase() + segment.slice(1).replace(/-/g, ' '), link: currentPath + '.html' })
    } else {
      const title = page.value.frontmatter?.title ||
        segment.charAt(0).toUpperCase() + segment.slice(1).replace(/-/g, ' ')
      crumbs.push({ text: title, link: currentPath + '.html' })
    }
  })

  return crumbs
})
</script>

<style scoped>
.breadcrumb {
  margin-bottom: 1rem;
  font-size: 0.875rem;
  color: var(--vp-c-text-2);
}

.breadcrumb ol {
  list-style: none;
  display: flex;
  flex-wrap: wrap;
  gap: 0.25rem;
  padding: 0;
  margin: 0;
}

.breadcrumb li {
  display: inline-flex;
  align-items: center;
}

.breadcrumb a {
  color: var(--vp-c-brand-1);
  text-decoration: none;
  transition: color 0.25s;
}

.breadcrumb a:hover {
  color: var(--vp-c-brand-2);
  text-decoration: underline;
}

.breadcrumb .current {
  color: var(--vp-c-text-1);
  font-weight: 500;
}

.breadcrumb .separator {
  margin: 0 0.375rem;
  color: var(--vp-c-text-3);
}
</style>
