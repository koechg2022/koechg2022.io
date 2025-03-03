


class manage_background_image {
    #image_location;
    #repeat;
    #size;
    #position;
    #font_color;


    constructor(path, repeat, size, position, font_color) {
        this.#image_location = path;
        this.#repeat = repeat;
        this.#size = size;
        this.#position = position;
        this.#font_color = font_color;
    }

    get_image_location() {
        return this.#image_location;
    }

    set_image_location(path) {
        this.#image_location = path;
    }

    get_repeat() {
        return this.#repeat;
    }

    set_repeat(repeat) {
        this.#repeat = repeat;
    }

    get_size() {
        return this.#size;
    }

    set_size(size) {
        this.#size = size;
    }

    get_position() {
        return this.#position;
    }

    set_position(position) {
        this.#position = position;
    }

    get_font_color() {
        return this.#font_color;
    }

    set_font_color(color) {
        this.#font_color = color;
    }

    get_style() {
        return `background-image : url('../files/${this.#image_location}');
                background-repeat : ${this.#repeat};
                background-size : ${this.#size};
                background-position : ${this.#position};
        `;
    }
    
}