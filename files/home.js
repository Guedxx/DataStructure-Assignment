async function GetCurseHomeList() {
    let response = await fetch('/getCursesHome', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({})
    });
    response = await response.json();
    console.log(response);

    let cursos = response.cursos;
    mainDiv.innerHTML = '';
    for (let curso of cursos) {
        mainDiv.innerHTML += curso;
    }
}