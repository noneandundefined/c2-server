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
                <a href="#" data-id="${release.id}">${
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

document.addEventListener('DOMContentLoaded', function () {
	fetchReleases();
});
