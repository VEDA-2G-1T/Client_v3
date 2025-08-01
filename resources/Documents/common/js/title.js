window.addEventListener('DOMContentLoaded', () => {
  const headTitleElement = document.querySelector('title')
  if (!headTitleElement) return

  const breadcrumbEl = document.querySelector('.list_path')
  const text = breadcrumbEl ? breadcrumbEl.innerText : ''
  const items = text.split('>').map((v) => v.trim())

  if (items.length === 1) {
    headTitleElement.innerText = 'SafetyNet Web Manual ' + items[0]
  } else if (items.length > 1) {
    headTitleElement.innerText =
      items[items.length - 1] + ' | SafetyNet Web Manual ' + items[0]
  }
})
