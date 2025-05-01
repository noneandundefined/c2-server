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

		if (releases.length > 0) {
			showReleaseDetails(releases[0]);
		}
	} catch (err) {
		console.error(err);
	}
}

function showReleaseDetails(release) {
	const name__tag = document.getElementById('name__tag');
	const date__release = document.getElementById('date__release');
	const description__release = document.getElementById(
		'description__release'
	);

	name__tag.innerText = release.name || release.tag_name;

	const isoDate = release.published_at;
	const date = new Date(isoDate);

	const month = String(date.getUTCMonth() + 1).padStart(2, '0'); // месяцы с 0
	const day = String(date.getUTCDate()).padStart(2, '0');
	const year = date.getUTCFullYear();

	date__release.innerText = `${month}/${day}/${year}`;
	const markdownText = release.body
		? release.body.replace(/\n/g, '<br>')
		: 'Нет описания';

	const converter = new showdown.Converter();
	const html = converter.makeHtml(markdownText);
	description__release.innerHTML = html;

	document.getElementById('windows__assets').innerHTML = `
        ${
			release.assets
				.map(
					(asset) => `
            <a href="${asset.browser_download_url}" target="_blank">
              ${asset.name} (${(asset.size / 1024).toFixed(1)} KB)
            </a>
        `
				)
				.join('') || '<li>Нет прикреплённых файлов</li>'
		}
    `;
}

document.addEventListener('DOMContentLoaded', function () {
	fetchReleases();
});
