

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
                <p id="imovel-${imovel.id}-ID">ID: ${imovel.id}</p>
                <p id="imovel-${imovel.id}-Bairro">Bairro: ${imovel.bairro}</p>
                <p id="imovel-${imovel.id}-Tipo">Tipo: ${imovel.tipo}</p>
                <p id="imovel-${imovel.id}-Rua">Rua: ${imovel.rua}</p>
                <p id="imovel-${imovel.id}-CEP">CEP: ${imovel.cep}</p>
                <p id="imovel-${imovel.id}-Número">Número: ${imovel.numero}</p>
                <p id="imovel-${imovel.id}-Preco-Total">Preço Total: ${imovel.precoTotal}</p>
                <p id="imovel-${imovel.id}-Preco-metro-quadrado">Preço metro quadrado: ${imovel.precoMetroQ}</p>
                <p id="imovel-${imovel.id}-Latitude">Latitude: ${imovel.latitude}</p>
                <p id="imovel-${imovel.id}-Longitude">Longitude: ${imovel.longitude}</p>
                <div class="imovel_s_description">
                    <h2>Descrição:</h2>
                    <p id="imovel-${imovel.id}-descricao">${imovel.descricao}</p>
                </div>
                
                <div class="imovel_s_buttons">
                    <button onclick="editImovel(${imovel.id})">
                        <svg xmlns="http://www.w3.org/2000/svg" height="24px" viewBox="0 -960 960 960" width="24px" fill="#ED1C24"><path d="M200-200h57l391-391-57-57-391 391v57Zm-80 80v-170l528-527q12-11 26.5-17t30.5-6q16 0 31 6t26 18l55 56q12 11 17.5 26t5.5 30q0 16-5.5 30.5T817-647L290-120H120Zm640-584-56-56 56 56Zm-141 85-28-29 57 57-29-28Z"/></svg>
                    </button>
                
                    <button onclick="deleteImovel(${imovel.id})">
                        <svg xmlns="http://www.w3.org/2000/svg" height="24px" viewBox="0 -960 960 960" width="24px" fill="#ED1C24"><path d="M280-120q-33 0-56.5-23.5T200-200v-520h-40v-80h200v-40h240v40h200v80h-40v520q0 33-23.5 56.5T680-120H280Zm400-600H280v520h400v-520ZM360-280h80v-360h-80v360Zm160 0h80v-360h-80v360ZM280-720v520-520Z"/></svg>
                    </button>
                </div>
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

function editImovel(imovelId) {
    // Obter as informações do imóvel pelo ID
    const imovelDiv = document.querySelector(`.imovel_s_buttons button[onclick="editImovel(${imovelId})"]`).closest('.imovel');

    // Extrair as informações da div
    const imovelInfo = {
        id: imovelId,
        bairro: document.getElementById(`imovel-${imovelId}-Bairro`).textContent.replace('Bairro: ', '').trim(),
        tipo: document.getElementById(`imovel-${imovelId}-Tipo`).textContent.replace('Tipo: ', '').trim(),
        rua: document.getElementById(`imovel-${imovelId}-Rua`).textContent.replace('Rua: ', '').trim(),
        cep: document.getElementById(`imovel-${imovelId}-CEP`).textContent.replace('CEP: ', '').trim(),
        numero: parseInt(document.getElementById(`imovel-${imovelId}-Número`).textContent.replace('Número: ', '').trim()) || -1,
        precoTotal: parseFloat(document.getElementById(`imovel-${imovelId}-Preco-Total`).textContent.replace('Preço Total: ', '').trim()),
        precoMetroQ: parseFloat(document.getElementById(`imovel-${imovelId}-Preco-metro-quadrado`).textContent.replace('Preço metro quadrado: ', '').trim()),
        latitude: parseFloat(document.getElementById(`imovel-${imovelId}-Latitude`).textContent.replace('Latitude: ', '').trim()),
        longitude: parseFloat(document.getElementById(`imovel-${imovelId}-Longitude`).textContent.replace('Longitude: ', '').trim()),
        descricao: document.getElementById(`imovel-${imovelId}-descricao`).textContent.trim(),
    };


    document.getElementById('imovel-edit').style.display = 'flex';

    // Preencher o formulário com as informações do imóvel
    document.getElementById('idEdit').value = imovelInfo.id;
    document.getElementById('bairroEdit').value = imovelInfo.bairro;
    document.getElementById('tipoEdit').value = imovelInfo.tipo;
    document.getElementById('ruaEdit').value = imovelInfo.rua || '';
    document.getElementById('numeroEdit').value = imovelInfo.numero;
    document.getElementById('precoTotalEdit').value = imovelInfo.precoTotal;
    document.getElementById('precoMetroQEdit').value = imovelInfo.precoMetroQ;
    document.getElementById('cepEdit').value = imovelInfo.cep;
    document.getElementById('latitudeEdit').value = imovelInfo.latitude;
    document.getElementById('longitudeEdit').value = imovelInfo.longitude;
    document.getElementById('descricaoEdit').value = imovelInfo.descricao || '';
}

async function editar() {
    const form = document.getElementById('imovelFormEdit');
    const formData = new FormData(form);
    const jsonData = {};

    formData.forEach((value, key) => {
        jsonData[key] = key === 'precoMetroQ' || key === 'precoTotal' || key === 'latitude' || key === 'longitude' ? parseFloat(value) :
            key === 'id' || key === 'numero' ? parseInt(value) : value;
    });

    await deleteImovel(jsonData.id);

    fetch('/submit_imovel', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(jsonData)
    }).then(response => {
        if (response.ok) {
            alert('Imóvel editado com sucesso!');
            document.getElementById('imovel-edit').style.display = 'none';
        } else {
            alert('Erro ao editar imóvel.');
        }
    }).catch(error => {
        console.error('Erro:', error);
        alert('Erro ao enviar os dados.');
    });
}



