

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
                <p>CEP: ${imovel.cep}</p>
                <p>Número: ${imovel.numero}</p>
                <p>Preço Total: ${imovel.precoTotal}</p>
                <p>Preço metro quadrado: ${imovel.precoMetroQ}</p>
                <p>Latitude: ${imovel.latitude}</p>
                <p>Longitude: ${imovel.longitude}</p>
                <div class="imovel_s_description">
                    <h2>Descrição:</h2>
                    <p>${imovel.descricao}</p>
                </div>
                <button onclick="deleteImovel(${imovel.id})">
                    <svg xmlns="http://www.w3.org/2000/svg" height="24px" viewBox="0 -960 960 960" width="24px" fill="#ED1C24"><path d="M280-120q-33 0-56.5-23.5T200-200v-520h-40v-80h200v-40h240v40h200v80h-40v520q0 33-23.5 56.5T680-120H280Zm400-600H280v520h400v-520ZM360-280h80v-360h-80v360Zm160 0h80v-360h-80v360ZM280-720v520-520Z"/></svg>
                </button>
            `;
            imovelDiv.id = `imovel-${imovel.id}`;
            resultsContainer.appendChild(imovelDiv);
        });
    } else {
        resultsContainer.innerHTML = '<p>Nenhum imóvel encontrado.</p>';
        document.getElementById('deleteAllButton').style.display = 'none';
    }
}

async function deleteAll(){
    const imovel_divs = document.getElementsByClassName('imovel_s');
    const ids = [];

    for (const imovel_div of imovel_divs) {
        const id = imovel_div.id.split('-')[1];
        ids.push(parseInt(id));
    }


    for (const id of ids) {
        await deleteImovel(id);
        await new Promise(r => setTimeout(r, 3));
    }
    document.getElementById('results').innerHTML = '<p>Todos os imóveis foram deletados.</p>';
}

async function deleteImovel(id) {
    const response = await fetch(`/delete_imovel`, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({id: id})
    });

    if (response.ok) {
        console.log('Imóvel deletado com sucesso!');
        // Remove the element from the page
        document.getElementById(`imovel-${id}`).remove();
    } else {
        console.log('Erro ao deletar imóvel.');
    }
}


