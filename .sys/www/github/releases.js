const GITHUB_HTTP_RELEASES = `https://api.github.com/repos/noneandundefined/icu/releases`;

async function fetchReleases() {
	try {
		const res = await fetch(GITHUB_HTTP_RELEASES);
		if (!res.ok) throw new Error('Error load');
		const releases = await res.json();

		const listEl = document.getElementById('releases-list');
		listEl.innerHTML = releases
			.map(
				(release) => `
            <li>
                <a id="btn__release" href="#" data-id="${release.id}">${
					release.name || release.tag_name
				}</a>
            </li>
        `
			)
			.join('');

		listEl.addEventListener('click', (event) => {
			if (event.target.tagName === 'A') {
				event.preventDefault();
				const releaseId = event.target.getAttribute('data-id');
				const release = releases.find((r) => r.id == releaseId);
				showReleaseDetails(release);
			}
		});
	} catch (err) {
		console.error(err);
	}
}

function showReleaseDetails(release) {
	const detailsEl = document.getElementById('release__details');
	detailsEl.innerHTML = `
      <h2>${release.name || release.tag_name}</h2>
      <p><strong>Tag:</strong> ${release.tag_name}</p>
      <p><strong>Описание:</strong><br>${
			release.body ? release.body.replace(/\n/g, '<br>') : 'Нет описания'
		}</p>
      <h3>Файлы:</h3>
      <ul>
        ${
			release.assets
				.map(
					(asset) => `
          <li>
            <a href="${asset.browser_download_url}" target="_blank">
              ${asset.name} (${(asset.size / 1024).toFixed(1)} KB)
            </a>
          </li>
        `
				)
				.join('') || '<li>Нет прикреплённых файлов</li>'
		}
      </ul>
    `;
}

document.addEventListener('DOMContentLoaded', function () {
	fetchReleases();
});
