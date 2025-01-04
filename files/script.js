

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

function pesquisar() {
    const form = document.getElementById('searchForm');
    const formData = new FormData(form);
    const data = Object.fromEntries(formData.entries());

    // Set default values for empty fields
    data.id = data.id || -1;
    data.bairro = data.bairro || '!NULL!';
    data.rua = data.rua || '!NULL!';
    data.tipo = data.tipo || '!NULL!';
    data.cep = data.cep || '!NULL!';
    data.precoMin = data.precoMin || '-inf';
    data.precoMax = data.precoMax || '+inf';
    data.latitude_min = data.latitude_min || '-inf';
    data.longitude_min = data.longitude_min || '-inf';
    data.latitude_max = data.latitude_max || '+inf';
    data.longitude_max = data.longitude_max || '+inf';

    fetch('/search_imoveis', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    })
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok ' + response.statusText);
            }
            return response.json();
        })
        .then(data => {
            renderResults(data);
        })
        .catch(error => {
            console.error('There was a problem with the fetch operation:', error);
        });
}

function renderResults(imoveis) {
    const resultsContainer = document.getElementById('results');
    resultsContainer.innerHTML = '';

    if (imoveis.length > 0) {
        document.getElementById('deleteAllButton').style.display = 'block';

        imoveis.forEach(imovel => {
            const imovelDiv = document.createElement('div');
            imovelDiv.className = 'imovel_s';

            imovelDiv.innerHTML = `
                <p>ID: ${imovel.id}</p>
                <p>Bairro: ${imovel.bairro}</p>
                <p>Tipo: ${imovel.tipo}</p>
                <p>Rua: ${imovel.rua}</p>
                <p>Número: ${imovel.numero}</p>
                <p>Preço Total: ${imovel.precoTotal}</p>
                <p>Preço metro quadrado: ${imovel.precoMetroQ}</p>
                <p>Latitude: ${imovel.latitude}</p>
                <p>Longitude: ${imovel.longitude}</p>
                <div class="imovel_s_description">
                    <h2>Descrição:</h2>
                    <p>${imovel.descricao}</p>
                </div>
                <button onclick="deleteImovel(${imovel.id})">Deletar</button>
            `;

            resultsContainer.appendChild(imovelDiv);
        });
    } else {
        resultsContainer.innerHTML = '<p>Nenhum imóvel encontrado.</p>';
        document.getElementById('deleteAllButton').style.display = 'none';
    }
}

function deleteAll(){
    fetch(`/delete_all`, {
        method: 'DELETE',
    }).then(response => {
        if (response.ok) {
            alert('Todos os imóveis foram apagados.');
            document.getElementById('results').innerHTML = '';
            this.style.display = 'none';
        } else {
            alert('Erro ao apagar todos os imóveis.');
        }
    }).catch(error => console.error('Erro:', error));
}

function deleteImovel(id) {
    fetch(`/delete_imovel/${id}`, {
        method: 'DELETE',
    }).then(response => {
        if (response.ok) {
            alert(`Imóvel ID ${id} apagado com sucesso.`);
            document.querySelector(`.imovel-item button[onclick="deleteImovel(${id})"]`).parentElement.remove();
            if (document.querySelectorAll('.imovel-item').length === 0) {
                document.getElementById('deleteAllButton').style.display = 'none';
            }
        } else {
            alert(`Erro ao apagar o imóvel ID ${id}.`);
        }
    }).catch(error => console.error('Erro:', error));
}


