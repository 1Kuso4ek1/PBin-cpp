const pasteData = JSON.parse(localStorage.getItem('pasteData'));
document.getElementById('paste-url').value = pasteData.url;
document.getElementById('delete-token').value = pasteData.deleteToken;

document.getElementById('copy-url').addEventListener('click', () => {
    navigator.clipboard.writeText(pasteData.url);
    alert('URL copied');
});

document.getElementById('copy-token').addEventListener('click', () => {
    navigator.clipboard.writeText(pasteData.deleteToken);
    alert('Token copied');
});

document.getElementById('view-paste').addEventListener('click', () => {
    window.location.href = pasteData.url;
});

document.getElementById('new-paste').addEventListener('click', () => {
    localStorage.removeItem('pasteData');
    window.location.href = '/';
});
