

function cadastrar() {
    const form = document.getElementById('imovelForm');
    const formData = new FormData(form);
    const jsonData = {};

    formData.forEach((value, key) => {
        jsonData[key] = key === 'precoMetroQ' || key === 'precoTotal' || key === 'latitude' || key === 'longitude' ? parseFloat(value) :
            key === 'id' || key === 'numero' ? parseInt(value) : value;
    });

    fetch('/submit_imovel', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(jsonData)
    }).then(response => {
        if (response.ok) {
            alert('Imóvel cadastrado com sucesso!');
        } else {
            alert('Erro ao cadastrar imóvel.');
        }
    }).catch(error => {
        console.error('Erro:', error);
        alert('Erro ao enviar os dados.');
    });
}

