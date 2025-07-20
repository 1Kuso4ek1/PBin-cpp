document.getElementById('delete-btn').addEventListener('click', () => {
    const token = document.getElementById('delete-token').value;
    if (!token) {
        alert('Please enter delete token');
        return;
    }

    fetch(window.location.pathname, {
        method: 'DELETE',
        headers: {'X-Delete-Token': token}
    })
        .then(response => {
            if (response.ok) {
                alert('Paste deleted');
                window.location.href = '/';
            } else {
                alert('Invalid token');
            }
        })
        .catch(error => {
            console.error('Error:', error);
            alert('Error deleting paste');
        });
});

// Apply Prism highlighting after page load
document.addEventListener('DOMContentLoaded', () => {
    if (typeof Prism !== 'undefined') {
        Prism.highlightAll();
    }
});
