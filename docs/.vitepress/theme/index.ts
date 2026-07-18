import DefaultTheme from 'vitepress/theme'
import Breadcrumb from './Breadcrumb.vue'
import './style.css'

export default {
  extends: DefaultTheme,
  enhanceApp({ app }) {
    app.component('Breadcrumb', Breadcrumb)
  }
}
