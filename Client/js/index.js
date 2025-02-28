


let first_visit = true;





const the_nav = `show-nav`;
const the_function = `the_function`;
const the_html = `the_html`;
const the_index = `the_index`;

const to_do = `TODO... Under Construction`;



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


const navigation = document.getElementById('nav_ul').getElementsByTagName('li');

