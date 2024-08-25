

const the_nav = `show-nav`;
const the_function = `the_function`;
const the_html = `the_html`;
const the_index = `the_index`;






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







const navigation = document.getElementById('nav_ul').getElementsByTagName('li');

const open_button = document.getElementById('open');
const close_button = document.getElementById('close');
const the_container = document.querySelector('.container');


function navigate_to_homepage() {
    console.log(`Directing back to home-page.`);
    // TODO IMPLEMENT ME
}

function navigate_to_aboutpage() {
    console.log(`Directing to about page`);
    // TODO IMPLEMENT ME
}

function navigate_to_githubpage() {
    console.log(`Directing to github page`);
    // TODO IMPLEMENT ME
}

function navigate_to_bookspage() {
    console.log(`Directing to book recs page`);
    // TODO IMPLEMENT ME
}

function navigate_to_photographypage() {
    console.log(`Directing to photography page`);
    // TODO IMPLEMENT ME
}

function navigate_to_travelpage() {
    console.log(`Directing to travel page`);
    // TODO IMPLEMENT ME
}

function navigate_to_hobbiespage() {
    console.log(`Directing to hobbies page`);
    // TODO IMPLEMENT ME
}

function navigate_to_contactpage() {
    console.log(`Directing to contact page`);
    // TODO IMPLEMENT ME
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

for (const nav in nav_objects) {
    console.log(`${nav} : ${nav_objects[nav]}`);
    for (const this_attr in nav_objects[nav]) {
        console.log(`\t${this_attr} : ${nav_objects[nav][this_attr]}`);
    }
}