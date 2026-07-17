import { defineConfig } from 'vitepress'

export default defineConfig({
    title: 'argparse-c',
    description:
            'Production-quality C/C++ argument parsing library for modern applications',
    lang: 'en-US',

    srcDir: '.',

    lastUpdated: true,
    cleanUrls: true,

    head: [
      ['meta', { name: 'author', content: 'Muhammad Fiaz' }],
      [
        'meta',
        {
          name: 'keywords',
          content:
            'argparse, C, C++, argument parsing, command-line, CLI, parser library'
        }
      ],
      ['meta', { name: 'robots', content: 'index, follow' }],

      // Favicon
      ['link', { rel: 'icon', type: 'image/svg+xml', href: '/favicon.svg' }],
      ['link', { rel: 'icon', type: 'image/png', sizes: '32x32', href: '/favicon-32x32.png' }],
      ['link', { rel: 'icon', type: 'image/png', sizes: '16x16', href: '/favicon-16x16.png' }],
      ['link', { rel: 'apple-touch-icon', sizes: '180x180', href: '/apple-touch-icon.png' }],
      ['link', { rel: 'manifest', href: '/manifest.json' }],
      ['meta', { name: 'theme-color', content: '#2563eb' }],

      // Open Graph
      ['meta', { property: 'og:type', content: 'website' }],
      ['meta', { property: 'og:site_name', content: 'argparse-c' }],
      [
        'meta',
        {
          property: 'og:title',
          content: 'argparse-c - Production-Quality C/C++ Argument Parsing'
        }
      ],
      [
        'meta',
        {
          property: 'og:description',
          content:
            'A production-quality C/C++ argument parsing library for modern applications'
        }
      ],
      [
        'meta',
        {
          property: 'og:url',
          content: 'https://muhammad-fiaz.github.io/argparse-c/'
        }
      ],
      [
        'meta',
        {
          property: 'og:image',
          content:
            'https://muhammad-fiaz.github.io/argparse-c/og-image.png'
        }
      ],

      // Twitter Card
      ['meta', { name: 'twitter:card', content: 'summary_large_image' }],
      ['meta', { name: 'twitter:site', content: '@muhammadfiaz_' }],
      ['meta', { name: 'twitter:creator', content: '@muhammadfiaz_' }],
      [
        'meta',
        {
          name: 'twitter:title',
          content: 'argparse-c - Production-Quality C/C++ Argument Parsing'
        }
      ],
      [
        'meta',
        {
          name: 'twitter:description',
          content:
            'A production-quality C/C++ argument parsing library for modern applications'
        }
      ],

      // Google Analytics
      [
        'script',
        {
          async: 'true',
          src: 'https://www.googletagmanager.com/gtag/js?id=G-6BVYCRK57P'
        }
      ],
      [
        'script',
        {},
        `window.dataLayer = window.dataLayer || [];
function gtag(){dataLayer.push(arguments);}
gtag('js', new Date());
gtag('config', 'G-6BVYCRK57P');`
      ],

      // Google Tag Manager
      [
        'script',
        {},
        `(function(w,d,s,l,i){w[l]=w[l]||[];w[l].push({'gtm.start':
new Date().getTime(),event:'gtm.js'});var f=d.getElementsByTagName(s)[0],
j=d.createElement(s),dl=l!='dataLayer'?'&l='+l:'';j.async=true;j.src=
'https://www.googletagmanager.com/gtm.js?id='+i+dl;f.parentNode.insertBefore(j,f);
})(window,document,'script','dataLayer','GTM-P4M9T8ZR');`
      ],

      // Google AdSense
      [
        'script',
        {
          async: 'true',
          src: 'https://pagead2.googlesyndication.com/pagead/js/adsbygoogle.js?client=ca-pub-2040560600290490',
          crossorigin: 'anonymous'
        }
      ],

      // JSON-LD Structured Data
      [
        'script',
        { type: 'application/ld+json' },
        JSON.stringify({
          '@context': 'https://schema.org',
          '@type': 'SoftwareSourceCode',
          name: 'argparse-c',
          description:
      'Production-quality C/C++ argument parsing library for modern applications',
          url: 'https://muhammad-fiaz.github.io/argparse-c/',
          codeRepository: 'https://github.com/muhammad-fiaz/argparse-c',
          author: {
            '@type': 'Person',
            name: 'Muhammad Fiaz',
            url: 'https://muhammadfiaz.com',
            sameAs: [
              'https://github.com/muhammad-fiaz',
              'https://linkedin.com/in/muhammad-fiaz-',
              'https://x.com/muhammadfiaz_'
            ]
          },
          programmingLanguage: ['C', 'C++'],
          license: 'https://opensource.org/licenses/MIT',
          keywords: [
            'argparse',
            'argument parsing',
            'command-line',
            'CLI',
            'C library'
          ]
        })
      ]
    ],

    themeConfig: {
      logo: '/logo.svg',
      siteTitle: 'argparse-c',

      nav: [
        { text: 'Home', link: '/' },
        { text: 'Guide', link: '/guide/getting-started' },
        { text: 'API', link: '/api/overview' },
        { text: 'Examples', link: '/examples/' },
        {
          text: 'Support',
          items: [
            {
              text: 'Sponsor',
              link: 'https://github.com/sponsors/muhammad-fiaz'
            },
            {
              text: 'Donate',
              link: 'https://buymeacoffee.com/muhammadfiaz'
            }
          ]
        },
        {
          text: 'v0.1.0',
          items: [
            { text: 'Changelog', link: '/guide/changelog' },
            { text: 'Contributing', link: '/guide/contributing' }
          ]
        }
      ],

      sidebar: [
        {
          text: 'Introduction',
          items: [
            { text: 'Getting Started', link: '/guide/getting-started' },
            { text: 'Installation', link: '/guide/installation' },
            { text: 'Quick Start', link: '/guide/quick-start' }
          ]
        },
        {
          text: 'Guide',
          items: [
            { text: 'Overview', link: '/guide/overview' },
            { text: 'Parser', link: '/guide/parser' },
            { text: 'Arguments', link: '/guide/arguments' },
            { text: 'Options', link: '/guide/options' },
            { text: 'Positional', link: '/guide/positional' },
            { text: 'Types', link: '/guide/types' },
            { text: 'Actions', link: '/guide/actions' },
            { text: 'Validators', link: '/guide/validators' },
            { text: 'Groups', link: '/guide/groups' },
            { text: 'Subcommands', link: '/guide/subcommands' },
            { text: 'Help Formatting', link: '/guide/help-formatting' },
            { text: 'Error Handling', link: '/guide/error-handling' },
            { text: 'Memory Management', link: '/guide/memory' },
            { text: 'Environment Variables', link: '/guide/env-vars' },
            { text: 'Config Files', link: '/guide/config-files' },
            { text: 'Shell Completion', link: '/guide/shell-completion' },
            { text: 'Unicode', link: '/guide/unicode' },
            { text: 'Custom Allocators', link: '/guide/custom-allocators' },
            { text: 'Performance', link: '/guide/performance' },
            { text: 'Internals', link: '/guide/internals' },
            { text: 'Advanced Usage', link: '/guide/advanced' }
          ]
        },
        {
          text: 'API Reference',
          items: [
            { text: 'Overview', link: '/api/overview' },
            { text: 'Parser', link: '/api/parser' },
            { text: 'Options', link: '/api/options' },
            { text: 'Namespace / Result', link: '/api/namespace' },
            { text: 'Groups', link: '/api/groups' },
            { text: 'Commands', link: '/api/commands' },
            { text: 'Validators', link: '/api/validators' },
            { text: 'Actions', link: '/api/actions' },
            { text: 'Formatting', link: '/api/format' },
            { text: 'Memory', link: '/api/memory' },
            { text: 'Errors', link: '/api/errors' },
            { text: 'Config', link: '/api/config' },
            { text: 'Completion', link: '/api/completion' },
            { text: 'Version', link: '/api/version' }
          ]
        },
        {
          text: 'Examples',
          items: [
            { text: 'Overview', link: '/examples/' },
            { text: 'Basic Usage', link: '/examples/basic' },
            { text: 'Flags & Switches', link: '/examples/flags' },
            { text: 'Subcommands', link: '/examples/subcommands' },
            { text: 'Validators', link: '/examples/validators' },
            { text: 'Groups', link: '/examples/groups' },
            { text: 'Arrays & Lists', link: '/examples/arrays' },
            { text: 'C++ Integration', link: '/examples/cpp' },
            { text: 'Large CLI Apps', link: '/examples/large-cli' }
          ]
        },
        {
          text: 'Resources',
          items: [
            { text: 'FAQ', link: '/guide/faq' },
            { text: 'Contributing', link: '/guide/contributing' }
          ]
        }
      ],

      socialLinks: [
        { icon: 'github', link: 'https://github.com/muhammad-fiaz/argparse-c' },
        { icon: 'linkedin', link: 'https://linkedin.com/in/muhammad-fiaz-' },
        { icon: 'x', link: 'https://x.com/muhammadfiaz_' }
      ],

      editLink: {
        pattern:
          'https://github.com/muhammad-fiaz/argparse-c/edit/main/docs/:path',
        text: 'Edit this page on GitHub'
      },

      footer: {
        message: 'Released under the MIT License.',
        copyright: 'Copyright © 2026 Muhammad Fiaz'
      },

      search: {
        provider: 'local'
      },

      carbonAds: {
        code: 'ca-pub-2040560600290490',
        placement: 'argparse-c'
      }
    },

    sitemap: {
      hostname: 'https://muhammad-fiaz.github.io/argparse-c'
    }
  })
