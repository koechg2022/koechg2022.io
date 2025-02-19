


let first_visit = true;





const the_nav = `show-nav`;
const the_function = `the_function`;
const the_html = `the_html`;
const the_index = `the_index`;
const homepage_html = `../html/__home__.html`;
const aboutpage_html = "../html/__about__.html";

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


const nav_tag = document.getElementById('nav_ul');

const navigation = document.getElementById('nav_ul').getElementsByTagName('li');

const open_button = document.getElementById('open');
const close_button = document.getElementById('close');
const page_content_container = document.querySelector('.page_content-container')
const page_content_header = document.querySelector('.page_content-header');
const page_content_content = document.querySelector('.page_content_content');
const the_container = document.querySelector('.container');


const navigate_to_homepage = async () => {
    page_content_content.innerHTML = "";
    console.log(`Directing back to home-page.`);
    the_container.classList.remove(the_nav);
    nav_tag.classList.remove(the_nav);
    page_content_header.innerText = `${page_titles["home_page"][Math.floor((Math.random() * page_titles["home_page"].length))]}`;
    page_content_content.innerHTML = await get_file_data(homepage_html);
}

const navigate_to_aboutpage = async () =>{
    console.log(`Directing to about page`);
    the_container.classList.remove(the_nav);
    nav_tag.classList.remove(the_nav);
    page_content_header.innerText = `${page_titles["about_page"][Math.floor(Math.random() * page_titles["about_page"].length)]}`;
    page_content_content.innerHTML = await get_file_data(aboutpage_html);
}

const navigate_to_githubpage = () => {
    console.log(`Directing to github page`);
    the_container.classList.remove(the_nav);
    page_content_header.innerText = `${page_titles["github_page"][Math.floor(Math.random() * page_titles["github_page"].length)]}`;
    nav_tag.classList.remove(the_nav);
}

const navigate_to_bookspage = () => {
    console.log(`Directing to book recs page`);
    the_container.classList.remove(the_nav);
    page_content_header.innerText = `${page_titles["bookrec_page"][Math.floor(Math.random() * page_titles["bookrec_page"].length)]}`;
    nav_tag.classList.remove(the_nav);
}

const navigate_to_photographypage = () => {
    console.log(`Directing to photography page`);
    the_container.classList.remove(the_nav);
    page_content_header.innerText = `${page_titles["photograpy_page"][Math.floor(Math.random() * page_titles["photograpy_page"].length)]}`;
    nav_tag.classList.remove(the_nav);
}

const navigate_to_travelpage = () => {
    console.log(`Directing to travel page`);
    the_container.classList.remove(the_nav);
    page_content_header.innerText = `${page_titles["travel_page"][Math.floor(Math.random() * page_titles["travel_page"].length)]}`;
    nav_tag.classList.remove(the_nav);
}

const navigate_to_hobbiespage = () => {
    console.log(`Directing to hobbies page`);
    the_container.classList.remove(the_nav);
    page_content_header.innerText = `${page_titles["hobbies_page"][Math.floor(Math.random() * page_titles["hobbies_page"].length)]}`;
    nav_tag.classList.remove(the_nav);
}

const navigate_to_contactpage = () => {
    console.log(`Directing to contact page`);
    the_container.classList.remove(the_nav);
    page_content_header.innerText = `${page_titles["contact_page"][Math.floor(Math.random() * page_titles["contact_page"].length)]}`;
    nav_tag.classList.remove(the_nav);
}

const nav_objects = (() => {
    
    let the_answer = {};
    let index;
    for (index = 0; index < navigation.length; index++) {
        if (navigation[index].innerText === undefined) {
            continue;
        }
        
        the_answer[navigation[index].innerText] = new Object();
        the_answer[navigation[index].innerText][the_html] = navigation[index];
        the_answer[navigation[index].innerText][the_function] = ((page) => {
            // console.log(`page is ${page}`);
            if (same_string(navigation[page].innerText, `home`)) {
                // console.log(`Adding...`);
                return navigate_to_homepage;
            }
            else if (same_string(navigation[page].innerText, `about`)) {
                // console.log(`Adding...`);
                return navigate_to_aboutpage;
            }
            else if (same_string(navigation[page].innerText, `github`)) {
                // console.log(`Adding...`);
                return navigate_to_githubpage
            }
            else if (same_string(navigation[page].innerText, `book recs`)) {
                // console.log(`Adding...`);
                return navigate_to_bookspage;
            }
            else if (same_string(navigation[page].innerText, `photography`)) {
                // console.log(`Adding...`);
                return navigate_to_photographypage;
            }
            else if (same_string(navigation[page].innerText, `travel`)) {
                // console.log(`Adding...`);
                return navigate_to_travelpage;
            }
            else if (same_string(navigation[page].innerText, `hobbies`)) {
                // console.log(`Adding...`);
                return navigate_to_hobbiespage;
            }
            else if (same_string(navigation[page].innerText, `contact`)) {
                // console.log(`Adding...`);
                return navigate_to_contactpage;
            }
            else {
                // console.log(`Error, not found`);
                return undefined;
            }
            })(index);
        the_answer[navigation[index].innerText][the_index] = index;
        
    }
    
    return the_answer;
})();


function add_navigation() {
    // console.log(`Inside add_navigation`);
    for (nav in nav_objects) {
        nav_objects[nav][the_html].addEventListener(`click`, nav_objects[nav][the_function]);
    }
}

function remove_navigation() {
    for (nav in nav_objects) {
        nav_objects[nav][the_html].removeEventListener(`click`, nav_objects[nav][the_function]);
    }
}

open_button.addEventListener(`click`, () => {
    the_container.classList.add(the_nav);
    add_navigation();
    nav_tag.classList.add(the_nav);
});

close_button.addEventListener(`click`, () => {
    the_container.classList.remove(the_nav);
    remove_navigation();
    nav_tag.classList.remove(the_nav);
});


(first_visit) ? navigate_to_homepage() : null;
// navigate_to_homepage();
first_visit = false;