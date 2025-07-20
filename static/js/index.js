document.addEventListener('DOMContentLoaded', function() {
    const textarea = document.getElementById('content');

    textarea.addEventListener('keydown', function(e) {
        if (e.key === 'Tab') {
            e.preventDefault();

            const start = this.selectionStart;
            const end = this.selectionEnd;

            // Вставляем 4 пробела
            this.value = this.value.substring(0, start) +
                '    ' +
                this.value.substring(end);

            // Устанавливаем курсор после вставленных пробелов
            this.selectionStart = this.selectionEnd = start + 4;
        }
    });
});


document.getElementById('paste-form').addEventListener('submit', async (e) => {
    e.preventDefault();

    const content = document.getElementById('content').value;
    const language = document.getElementById('language').value;
    const expiration = document.getElementById('expiration').value;

    try {
        const response = await fetch('/paste', {
            method: 'POST',
            headers: {'Content-Type': 'application/json'},
            body: JSON.stringify({
                text: content,
                language: language,
                ttl_seconds: expiration
            })
        });

        const data = await response.json();
        localStorage.setItem('pasteData', JSON.stringify(data));
        window.location.href = '/created';
    } catch (error) {
        alert('Error creating paste');
    }
});
