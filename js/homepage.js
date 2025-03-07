






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

const pages = ["about", "books", "contact", "github", "hobbies", "homepage", "photography", "travel"];

const background_images = {
    beach : `${file_directory}${system_slash}dutch beach and fence.avif`,
    desert_road : `${file_directory}${system_slash}Desert road.jpg`,
    geoff_and_anna_one : `${file_directory}${system_slash}geoff anna and her roommate.JPG`,
    geoff_on_throne : `${file_directory}${system_slash}geoffrey on throne.JPG`,
    vlierhof_one : `${file_directory}${system_slash}vlierhof crew biking 1.JPG`,
    vlierhof_two : `${file_directory}${system_slash}vlierhof crew biking 2.JPG`,
    vlierhof_three : `${file_directory}${system_slash}vlierhof crew biking 3.JPG`,
    vlierhof_four : `${file_directory}${system_slash}vlierhof crew biking 4.JPG`
};

const page_options = {
    homepage : [
        background_images["geoff_on_throne"], background_images["beach"], background_images["desert_road"], 
        background_images["geoff_and_anna_one"], background_images["geoff_on_throne"], background_images["vlierhof_one"], 
        background_images["vlierhof_two"], background_images["vlierhof_three"], background_images["vlierhof_four"]],
    about : [
        background_images["beach"], background_images["desert_road"], 
        background_images["geoff_and_anna_one"], background_images["geoff_on_throne"], background_images["vlierhof_one"], 
        background_images["vlierhof_two"], background_images["vlierhof_three"], background_images["vlierhof_four"]],
    github : [
        background_images["beach"], background_images["desert_road"], background_images["vlierhof_one"], 
        background_images["vlierhof_two"], background_images["vlierhof_three"], background_images["vlierhof_four"]],
    hobbies : [
        background_images["geoff_on_throne"], background_images["beach"], background_images["desert_road"], 
        background_images["geoff_and_anna_one"], background_images["geoff_on_throne"], background_images["vlierhof_one"], 
        background_images["vlierhof_two"], background_images["vlierhof_three"], background_images["vlierhof_four"]],
    books : [
        background_images["geoff_on_throne"], background_images["beach"], background_images["desert_road"], 
        background_images["geoff_and_anna_one"], background_images["geoff_on_throne"], background_images["vlierhof_one"], 
        background_images["vlierhof_two"], background_images["vlierhof_three"], background_images["vlierhof_four"]],
    photography : [
        background_images["geoff_on_throne"], background_images["beach"], background_images["desert_road"], 
        background_images["geoff_and_anna_one"], background_images["geoff_on_throne"], background_images["vlierhof_one"], 
        background_images["vlierhof_two"], background_images["vlierhof_three"], background_images["vlierhof_four"]],
    travel : [
        background_images["geoff_on_throne"], background_images["beach"], background_images["desert_road"], 
        background_images["geoff_and_anna_one"], background_images["geoff_on_throne"], background_images["vlierhof_one"], 
        background_images["vlierhof_two"], background_images["vlierhof_three"], background_images["vlierhof_four"]],
    contact : [
        background_images["geoff_on_throne"], background_images["beach"], background_images["desert_road"], 
        background_images["geoff_and_anna_one"], background_images["geoff_on_throne"], background_images["vlierhof_one"], 
        background_images["vlierhof_two"], background_images["vlierhof_three"], background_images["vlierhof_four"]]
}

const navigation = document.getElementById('nav_ul').querySelectorAll('li');

const logo = document.getElementById('logo');

navigation.forEach((this_nav) => {
    let target_url = `/Client/html/${this_nav.innerText.toLowerCase()}.html`;
    if (this_nav.innerText.toLowerCase() === 'homepage') {
        this_nav.title = `Navigate to my homepage`;
        logo.title = `${this_nav.title}`;
        logo.addEventListener('click', () => {
            window.location.href = target_url;
        });
    }
    else if (this_nav.innerText.toLowerCase() === 'about') {
        this_nav.title =  `About page. Learn a bit about Geoffrey`;
    }
    else if (this_nav.innerText.toLowerCase() === 'github') {
        this_nav.title = `Check out my projects`;
    }
    else if (this_nav.innerText.toLowerCase() === 'book recs') {
        this_nav.title = `Check out the books I'm reading and my reviews of them`;
    }
    else if (this_nav.innerText.toLowerCase() === 'photography') {
        this_nav.title = `Check out my photographs`;
    }
    else if (this_nav.innerText.toLowerCase() === 'travel') {
        this_nav.title = `Check out my travel photos and blogs`;
    }
    else if (this_nav.innerText.toLowerCase() === 'hobbies') {
        this_nav.title = `Swimming, Football, Guitar, Learning...`;
    }
    else if (this_nav.innerText.toLowerCase() === 'contact') {
        this_nav.title = `Reach out to me`;
    }
    this_nav.addEventListener('click', () => {
        window.location.href = target_url;
    });
});

const page_title = document.querySelector('.page_title');

const page_titles = {
    homepage : [`Swimmer`, `Reader`, `Programmer`, `Backpacker`],
    about : [`About Geoffrey`],
    github : [`Projects`],
    books : [`Books`],
    photography : [`Photos`],
    travel : [`Travel Photos and Blog`],
    hobbies : [`Reading`, `Swimming`, `Sleeping`, `Coding`, `Traveling`],
    contact : [`How to reach me`]
};

page_title.innerText = page_titles[current_page_name()][Math.floor(Math.random() * page_titles[current_page_name()].length)];


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

function current_page_name() {
    const path = window.location.pathname;
    const the_answer = path.split("/").pop().replace(/\.html$/, '');
    return the_answer;
}