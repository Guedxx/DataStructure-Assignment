let LoginNameDiv;
let LoginButtonsDiv;
let UserNameSpan;
let mainDiv;

async function UpdateLoginDisplay() {
    let name = localStorage.getItem('LoginName');
    if (name) {
        UserNameSpan.innerHTML = name;
        LoginNameDiv.style.display = 'block';
        LoginButtonsDiv.style.display = 'none';
        await ServerLogin();
    } else {
        LoginNameDiv.style.display = 'none';
        LoginButtonsDiv.style.display = 'block';
        if (!currentUrl.endsWith('/')) {
            window.location.href = '/';
        }
    }
}

async function Login() {
    let name = prompt('Entre com o seu nome: ');
    if (name) {
        localStorage.setItem('LoginName', name);
    }
    let password = prompt('Entre com a sua senha: ');
    if (password) {
        localStorage.setItem('LoginPassword', password);
    }

    await UpdateLoginDisplay();
}

async function ServerLogin() {
    let name = localStorage.getItem('LoginName');
    let password = localStorage.getItem('LoginPassword');
    if (!name || !password) {
        alert('Por favor, faça login primeiro');
        return;
    }

    let response = await fetch('/login', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({
            username: name,
            password: password
        })
    });
    response = await response.json();

    if (!response.loginError) {
        if (response.isAdmin && !currentUrl.includes('/admin')) {
            window.location.href = '/admin';
        }
    }
    else {
        await Logout();
        alert('Login falhou');
    }
}

async function ServerSingUp() {
    let name = localStorage.getItem('LoginName');
    let password = localStorage.getItem('LoginPassword');
    if (!name || !password) {
        alert('Erro desconhecido ao criar conta!');
        return;
    }

    let response = await fetch('/signUp', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({
            username: name,
            password: password
        })
    });
    response = await response.json();

    if (!response.signUpError) {
        alert('SingUp feito com sucesso');
        await ServerLogin();
    }
    else {
        await Logout();
        alert('SingUp falhou: há um usuário com esse nome já cadastrado!');
    }
}

async function SingUp() {
    let name = prompt('Entre com o seu nome: ');
    if (name) {
        localStorage.setItem('LoginName', name);
    }
    let password = prompt('Entre com a sua senha: ');
    if (password) {
        localStorage.setItem('LoginPassword', password);
    }

    await ServerSingUp();
}

async function Logout() {
    localStorage.removeItem('LoginName');
    localStorage.removeItem('LoginPassword');
    await UpdateLoginDisplay();
    window.location.href = '/';
}


const currentUrl = window.location.href;

document.addEventListener('DOMContentLoaded', async function() {
    LoginNameDiv = document.getElementById('LoginName');
    LoginButtonsDiv = document.getElementById('LoginButtons');
    UserNameSpan = document.getElementById('UserName');
    mainDiv = document.getElementById('main');

    await UpdateLoginDisplay();

    console.log("currentUrl: " + currentUrl)
    if (currentUrl.endsWith('/')) {
        await GetCurseHomeList();
    }
    else if (currentUrl.includes('/curse/')) {
        let curseName = currentUrl.split('/').pop();
        await GetCurse(curseName);
    }
    else if (currentUrl.includes('/modulo/')) {
        let url = currentUrl.split('/');
        let moduleName = url.pop();
        let curseName = url.pop();
        await GetModule(curseName, moduleName);
    }
    else if (currentUrl.includes('/admin')) {
        await LoadAdmin();
    }
    else {
        console.log('URL desconhecida: ' + currentUrl);
    }
});