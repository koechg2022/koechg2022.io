






const system_slash = (function() {
    const platform = navigator.platform.toLowerCase();
    if (platform.includes('win')) {
        return "\\";
    } else if (platform.includes('mac') || platform.includes('linux') || platform.includes('x11')) {
        return "/";
    } else {
        // Default to forward slash for unknown systems
        return "/";
    }
})();

const file_directory = `files`;

const navigation = document.getElementById('nav_ul').querySelectorAll('li');

navigation.forEach((navigation) => {
    let target_url = `${navigation.innerText.toLowerCase()}.html`;
    if (navigation.innerText.toLowerCase() === 'homepage') {
        navigation.title = `Navigate to my homepage`;
    }
    else if (navigation.innerText.toLowerCase() === 'about') {
        navigation.title =  `About page. Learn a bit about Geoffrey`;
    }
    else if (navigation.innerText.toLowerCase() === 'github') {
        navigation.title = `Check out my projects`;
    }
    else if (navigation.innerText.toLowerCase() === 'book recs') {
        navigation.title = `Check out the books I'm reading and my reviews of them`;
    }
    else if (navigation.innerText.toLowerCase() === 'photography') {
        navigation.title = `Check out my photographs`;
    }
    else if (navigation.innerText.toLowerCase() === 'travel') {
        navigation.title = `Check out my travel photos and blogs`;
    }
    else if (navigation.innerText.toLowerCase() === 'hobbies') {
        navigation.title = `Swimming, Football, Guitar, Learning...`;
    }
    else if (navigation.innerText.toLowerCase() === 'contact') {
        navigation.title = `Reach out to me`;
    }
    navigation.addEventListener('click', () => {
        window.location.href = target_url;
    });
});

const page_title = document.querySelector('.page_title');

const page_titles = {
    home_page : [`Swimmer`, `Reader`, `Programmer`, `Backpacker`],
    about_page : [`About Geoffrey`],
    github_page : [`Projects`],
    bookrec_page : [`Books`],
    photograpy_page : [`Photos`],
    travel_page : [`Travel Photos and Blog`],
    hobbies_page : [`Reading`, `Swimming`, `Sleeping`, `Coding`, `Traveling`],
    contact_page : [`How to reach me`]
};

page_title.innerText = page_titles.home_page[Math.floor(Math.random() * page_titles.home_page.length)];


function same_string(first_string, second_string, ignore_case = true) {
    first_string = String(first_string);
    second_string = String(second_string);
    if (first_string.length !== second_string.length) {
        return false;
    }
    let index;
    for (index = 0; index < first_string.length && index < second_string.length; index++) {
        if (ignore_case) {
            if (first_string[index].toLowerCase() !== second_string[index].toLowerCase()) {
                return false;
            }
            continue;
        }
        if (first_string[index] !== second_string[index]) {
            return false;
        }
    }
    return true;
}

function index_of_string(the_list, the_string, ignore_case = true) {
    let the_answer;
    for (the_answer = 0; the_answer < the_list.length; the_answer++) {
        if (same_string(the_list[the_answer], the_string, ignore_case)) {
            return the_answer;
        }
    }
    return -1;
}

function index_of(the_string, substring, ignore_case = true) {
    let index = 0;
    let test_string = "";
    for (index = 0; index < the_string.length; index++) {
        test_string = the_string.substring(index, Math.min(the_string.length(), index + substring.length));
        if (same_string(test_string, substring, ignore_case)) {
            return index;
        }
    }
    return -1;
}

async function get_file_data(file_name) {

    let the_answer = await fetch(file_name)
    .then((res) => {
        // console.log(`res is ${res.text()}`);
        return res.text();
    })
    .then(msg => {
        // console.log(`msg is ${msg}`);
        return msg;
    })
    .catch ((failure) => {
        console.error(`Ya it epically failed... Error ${failure.text}`);
        return "";
    });
    return the_answer;
}
