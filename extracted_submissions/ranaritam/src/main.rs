// Author: Ritam Rana

use std::*;
use std::fs::File;
use std::io::{Read, Write};

// header struct
#[derive(Debug, Clone)]
struct Header{
    id_length: u8,
    color_map_type: u8,
    data_type_code: u8,
    color_map_origin: u16,
    color_map_length: u16,
    color_map_depth: u8,
    x_origin: u16,
    y_origin: u16,
    width: u16,
    height: u16,
    bits_per_pixel: u8,
    image_descriptor: u8
}

// pixel struct in b,g,r format
#[derive(Debug, Clone)]
struct Pixel {
    b: u8,
    g: u8,
    r: u8,
}

// stores header data of a certain file
fn store_header(mut file: &File) -> Header {
    let mut header_data = [0; 18];

    file.read_exact(&mut header_data).expect("cry");

    return Header{
        id_length: header_data[0],
        color_map_type: header_data[1],
        data_type_code: header_data[2],
        color_map_origin: u16::from_le_bytes([header_data[3], header_data[4]]),
        color_map_length: u16::from_le_bytes([header_data[5], header_data[6]]),
        color_map_depth: header_data[7],
        x_origin: u16::from_le_bytes([header_data[8], header_data[9]]),
        y_origin: u16::from_le_bytes([header_data[10], header_data[11]]),
        width: u16::from_le_bytes([header_data[12], header_data[13]]),
        height: u16::from_le_bytes([header_data[14], header_data[15]]),
        bits_per_pixel: header_data[16],
        image_descriptor: header_data[17]
    }
}

// stores pixel data in a Vector of Pixels
fn store_pixels(file: &mut File, header: &Header) -> Vec<Pixel> {
    let pixel_count = (header.width as usize) * (header.height as usize);
    let mut pixel_data = Vec::with_capacity(pixel_count);

    for i in 0..pixel_count {
        let mut pixel_bytes = [0; 3];
        file.read_exact(&mut pixel_bytes).expect("rust is bad");

        let pixel = Pixel {
            b: pixel_bytes[0],
            g: pixel_bytes[1],
            r: pixel_bytes[2],
        };

        pixel_data.push(pixel);
    }

    return pixel_data
}

// writes the header data when given a header value and file
fn write_header(file: &mut File, header: &Header) {
    let mut header_data = Vec::new();
    header_data.push(header.id_length);
    header_data.push(header.color_map_type);
    header_data.push(header.data_type_code);
    header_data.extend_from_slice(&header.color_map_origin.to_le_bytes());
    header_data.extend_from_slice(&header.color_map_length.to_le_bytes());
    header_data.push(header.color_map_depth);
    header_data.extend_from_slice(&header.x_origin.to_le_bytes());
    header_data.extend_from_slice(&header.y_origin.to_le_bytes());
    header_data.extend_from_slice(&header.width.to_le_bytes());
    header_data.extend_from_slice(&header.height.to_le_bytes());
    header_data.push(header.bits_per_pixel);
    header_data.push(header.image_descriptor);

    file.write_all(&header_data).expect("akjslkdjf");
}

// writes pixel data when given a vector of pixels
fn write_pixels(file: &mut File, pixels: &Vec<Pixel>) {
    for pixel in pixels {
        file.write_all(&[pixel.b, pixel.g, pixel.r]).expect("Failed to write pixel data to file");
    }
}

// multiply function
fn multiply(header: &Header, pixel_one: &Vec<Pixel>, pixel_two: &Vec<Pixel>) -> Vec<Pixel>{
    let width = header.width as usize;
    let height = header.height as usize;

    let mut out_vec = Vec::with_capacity(width * height);

    for i in 0..pixel_one.len(){
        let out = Pixel{
            b: (((pixel_one[i].b as f32) * (pixel_two[i].b as f32))/255.0).round() as u8,
            g: (((pixel_one[i].g as f32) * (pixel_two[i].g as f32))/255.0).round() as u8,
            r: (((pixel_one[i].r as f32) * (pixel_two[i].r as f32))/255.0).round() as u8
        };

        out_vec.push(out);
    }

    return out_vec;
}

// subtract function
fn subtract(header: &Header, pixel_one: &Vec<Pixel>, pixel_two: &Vec<Pixel>) -> Vec<Pixel>{
    let width = header.width as usize;
    let height = header.height as usize;

    let mut out_vec = Vec::with_capacity(width * height);

    for i in 0..pixel_one.len(){
        let out = Pixel{
            b: clamp((pixel_one[i].b as i16 - pixel_two[i].b as i16), 0, 255),
            g: clamp((pixel_one[i].g as i16 - pixel_two[i].g as i16), 0, 255),
            r: clamp((pixel_one[i].r as i16 - pixel_two[i].r as i16), 0, 255)
        };

        out_vec.push(out);
    }

    return out_vec;
}

// screen function
fn screen(header: &Header, pixel_one: &Vec<Pixel>, pixel_two: &Vec<Pixel>) -> Vec<Pixel>{
    let width = header.width as usize;
    let height = header.height as usize;

    let mut out_vec = Vec::with_capacity(width * height);

    for i in 0..pixel_one.len(){
        let out = Pixel{
            b: 255 - (((((255 - pixel_one[i].b) as f32) * ((255 - pixel_two[i].b) as f32))/255.0).round() as u8),
            g: 255 - (((((255 - pixel_one[i].g) as f32) * ((255 - pixel_two[i].g) as f32))/255.0).round() as u8),
            r: 255 - (((((255 - pixel_one[i].r) as f32) * ((255 - pixel_two[i].r) as f32))/255.0).round() as u8)
        };

        out_vec.push(out);
    }

    return out_vec;
}

// overlay function
fn overlay(pixel_one: &Vec<Pixel>, pixel_two: &Vec<Pixel>) -> Vec<Pixel>{
    let mut overlaid_vec = Vec::with_capacity(pixel_one.len());

    for i in 0..pixel_one.len(){
        let mut modified_pixel = Pixel{
            b:0,
            g:0,
            r:0
        };

        let normalized_r = pixel_two[i].r as f32/255.0;
        let normalized_g = pixel_two[i].g as f32/255.0;
        let normalized_b = pixel_two[i].b as f32/255.0;

        let normalized_r1 = pixel_one[i].r as f32/255.0;
        let normalized_g1 = pixel_one[i].g as f32/255.0;
        let normalized_b1 = pixel_one[i].b as f32/255.0;

        let average = (normalized_r + normalized_g + normalized_b)/3.0;

        if average > 0.5 {
            modified_pixel.r = ((1.0 - 2.0 * (1.0 - normalized_r1) * (1.0 - normalized_r)) * 255.0).round() as u8;
            modified_pixel.g = ((1.0 - 2.0 * (1.0 - normalized_g1) * (1.0 - normalized_g)) * 255.0).round() as u8;
            modified_pixel.b = ((1.0 - 2.0 * (1.0 - normalized_b1) * (1.0 - normalized_b)) * 255.0).round() as u8;
        }

        else{
            modified_pixel.r = (2.0 * (normalized_r1) * (normalized_r) * 255.0).round() as u8;
            modified_pixel.g = (2.0 * (normalized_g1) * (normalized_g) * 255.0).round() as u8;
            modified_pixel.b = (2.0 * (normalized_b1) * (normalized_b) * 255.0).round() as u8;
        }

        overlaid_vec.push(modified_pixel);
    }

    return overlaid_vec;
}

// add 200 to channel function
fn add200(pixels: &Vec<Pixel>) -> Vec<Pixel> {
    let mut added_vec = Vec::with_capacity(pixels.len());

    for pixel in pixels {
        let added_g = clamp((pixel.g as i16 + 200), 0, 255);

        let added_pixel = Pixel {
            b: pixel.b,
            g: added_g,
            r: pixel.r,
        };

        added_vec.push(added_pixel);
    }

    return added_vec;
}

// multiply by four function
fn x4(pixels: &Vec<Pixel>) -> Vec<Pixel> {
    let mut x4_vec = Vec::with_capacity(pixels.len());

    for pixel in pixels {
        let multiplied_r = clamp((pixel.r as i16 * 4), 0, 255);
        let multiplied_b = clamp((pixel.b as i16 * 0), 0, 255);

        let modified_pixel = Pixel {
            b: multiplied_b,
            g: pixel.g,
            r: multiplied_r,
        };

        x4_vec.push(modified_pixel);
    }

    return x4_vec;
}

// write to a specific channel function
fn write_specific_channel(pixels: &Vec<Pixel>, channel: &str) -> Vec<Pixel> {
    let mut channel_vec = Vec::with_capacity(pixels.len());

    for pixel in pixels {
        let mut modified_pixel = Pixel{
            b:0,
            g:0,
            r:0
        };

        if channel == "r"{
            modified_pixel = Pixel {
                b: pixel.r,
                g: pixel.r,
                r: pixel.r,
            };
        }

        else if channel == "g"{
            modified_pixel = Pixel {
                b: pixel.g,
                g: pixel.g,
                r: pixel.g,
            };
        }

        else{
            modified_pixel = Pixel {
                b: pixel.b,
                g: pixel.b,
                r: pixel.b,
            };
        }

        channel_vec.push(modified_pixel);
    }

    return channel_vec;
}

// combine channels function
fn combine_channels(pixels_r: &Vec<Pixel>, pixels_g: &Vec<Pixel>, pixels_b: &Vec<Pixel>) -> Vec<Pixel>{
    let mut combined_vec = Vec::with_capacity(pixels_r.len());

    for i in 0..pixels_r.len(){
        let modified_pixel = Pixel{
            b: pixels_b[i].b,
            g: pixels_g[i].g,
            r: pixels_r[i].r
        };

        combined_vec.push(modified_pixel);
    }

    return combined_vec;
}

// ec function, puts four images in a quadrant
fn quadrants(header: &Header, pixel_one: &Vec<Pixel>, pixel_two: &Vec<Pixel>, pixel_three: &Vec<Pixel>, pixel_four: &Vec<Pixel>) -> Vec<Pixel> {
    let width = header.width as usize;
    let height = header.height as usize;

    let mut out_vec:Vec<Pixel> = Vec::with_capacity(width * height);

    for x in 0..height/2{
        for i in 0..width/2{
            out_vec.push(pixel_one[i + (x * width/2)].clone());
        }

        for i in 0..width/2{
            out_vec.push(pixel_two[i + (x * width/2)].clone());
        }
    }

    for x in 0..height/2{
        for i in 0..width/2{
            out_vec.push(pixel_three[i + (x * width/2)].clone());
        }

        for i in 0..width/2{
            out_vec.push(pixel_four[i + (x * width/2)].clone());
        }
    }

    return out_vec;
}

// rotates image
fn rotate_image(pixels: &Vec<Pixel>) -> Vec<Pixel>{
    let length = pixels.len();
    let mut rotated_vec = Vec::with_capacity(length);

    for i in 0..pixels.len(){
        rotated_vec.push(Pixel{
            b: pixels[length - i - 1].b,
            g: pixels[length - i - 1].g,
            r: pixels[length - i - 1].r
        })
    }

    return rotated_vec;
}

// clamps values
fn clamp(mut value: i16, min: i16, max: i16) -> u8 {
    if value < min {
        value = min;
    }
    else if value > max {
        value = max;
    }

    return value as u8;
}

// main function
fn main() {
    // task 1
    let mut layer1 = File::open("input/layer1.tga").expect("layah 1");
    let layer1_header = store_header(&mut layer1);
    let layer1_pixels = store_pixels(&mut layer1, &layer1_header);

    let mut pattern1 = File::open("input/pattern1.tga").expect("pattern 1");
    let pattern1_header = store_header(&mut pattern1);
    let pattern1_pixels = store_pixels(&mut pattern1, &pattern1_header);

    let mut result = multiply(&layer1_header, &layer1_pixels, &pattern1_pixels);

    let mut outfile = File::create("output/part1.tga").expect("part1");
    write_header(&mut outfile, &layer1_header);
    write_pixels(&mut outfile, &result);

    // task 2
    let mut layer2 = File::open("input/layer2.tga").expect("layah 2");
    let layer2_header = store_header(&mut layer2);
    let layer2_pixels = store_pixels(&mut layer2, &layer2_header);

    let mut car = File::open("input/car.tga").expect("car");
    let car_header = store_header(&mut car);
    let car_pixels = store_pixels(&mut car, &car_header);

    result = subtract(&layer2_header, &car_pixels, &layer2_pixels);

    outfile = File::create("output/part2.tga").expect("part2");
    write_header(&mut outfile, &layer1_header);
    write_pixels(&mut outfile, &result);

    // task 3
    let mut pattern2 = File::open("input/pattern2.tga").expect("pattern 2");
    let pattern2_header = store_header(&mut pattern2);
    let pattern2_pixels = store_pixels(&mut pattern2, &pattern2_header);

    let mut text = File::open("input/text.tga").expect("text");
    let text_header = store_header(&mut text);
    let text_pixels = store_pixels(&mut text, &text_header);

    result = multiply(&layer1_header, &layer1_pixels, &pattern2_pixels);
    result = screen(&text_header, &result, &text_pixels);

    outfile = File::create("output/part3.tga").expect("part3");
    write_header(&mut outfile, &text_header);
    write_pixels(&mut outfile, &result);

    // task 4
    let mut circle = File::open("input/circles.tga").expect("circle");
    let circle_header = store_header(&mut circle);
    let circle_pixels = store_pixels(&mut circle, &circle_header);

    result = multiply(&layer2_header, &layer2_pixels, &circle_pixels);
    result = subtract(&pattern2_header, &result, &pattern2_pixels);

    outfile = File::create("output/part4.tga").expect("part4");
    write_header(&mut outfile, &pattern2_header);
    write_pixels(&mut outfile, &result);

    //task 5
    result = overlay(&layer1_pixels,&pattern1_pixels);
    outfile = File::create("output/part5.tga").expect("part5");
    write_header(&mut outfile, &layer1_header);
    write_pixels(&mut outfile, &result);

    //task 6
    result = add200(&car_pixels);

    outfile = File::create("output/part6.tga").expect("part6");
    write_header(&mut outfile, &car_header);
    write_pixels(&mut outfile, &result);

    // task 7
    result = x4(&car_pixels);

    outfile = File::create("output/part7.tga").expect("part7");
    write_header(&mut outfile, &car_header);
    write_pixels(&mut outfile, &result);

    // task 8
    result = write_specific_channel(&car_pixels, "r");

    outfile = File::create("output/part8_r.tga").expect("part8r");
    write_header(&mut outfile, &car_header);
    write_pixels(&mut outfile, &result);

    result = write_specific_channel(&car_pixels, "g");

    outfile = File::create(&"output/part8_g.tga").expect("part8g");
    write_header(&mut outfile, &car_header);
    write_pixels(&mut outfile, &result);

    result = write_specific_channel(&car_pixels, &"b");

    outfile = File::create("output/part8_b.tga").expect("part8b");
    write_header(&mut outfile, &car_header);
    write_pixels(&mut outfile, &result);

    // task 9
    let mut layer_red = File::open("input/layer_red.tga").expect("layah 1");
    let mut layer_green = File::open("input/layer_green.tga").expect("layah 1");
    let mut layer_blue = File::open("input/layer_blue.tga").expect("layah 1");

    let red_header = store_header(&layer_red);
    let red_pixels = store_pixels(&mut layer_red, &red_header);

    let green_header = store_header(&layer_green);
    let green_pixels = store_pixels(&mut layer_green, &green_header);

    let blue_header = store_header(&layer_blue);
    let blue_pixels = store_pixels(&mut layer_blue, &blue_header);

    result = combine_channels(&red_pixels, &green_pixels, &blue_pixels);
    outfile = File::create(&"output/part9.tga").expect("part9");
    write_header(&mut outfile, &blue_header);
    write_pixels(&mut outfile, &result);

    // task 10
    let mut text2 = File::open("input/text2.tga").expect("text2");
    let text2_header = store_header(&text2);
    let text2_pixels = store_pixels(&mut text2, &text2_header);

    result = rotate_image(&text2_pixels);
    outfile = File::create(&"output/part10.tga").expect("part10");
    write_header(&mut outfile, &text2_header);
    write_pixels(&mut outfile, &result);

    // ec :)
    let mut quadrant_header = circle_header.clone();
    quadrant_header.height *= 2;
    quadrant_header.width *= 2;

    let quadrant_result = quadrants(&quadrant_header,&text_pixels, &pattern1_pixels, &car_pixels, &circle_pixels);

    outfile = File::create(&"output/extracredit.tga").expect("ec");
    write_header(&mut outfile, &quadrant_header);
    write_pixels(&mut outfile, &quadrant_result);

    // let mut input = File::open("examples/EXAMPLE_part5.tga").expect("circle");
    // let mut output = File::open("output/part5.tga").expect("circle");
    //
    // let input_header = store_header(&input);
    // let input_pixels = store_pixels(&mut input, &input_header);
    //
    // let output_header = store_header(&output);
    // let output_pixels = store_pixels(&mut output, &output_header);
    //
    // check(&input_pixels, &output_pixels);
}

fn check(input: &Vec<Pixel>, output: &Vec<Pixel>){
    let mut equal = true;

    for i in 0..input.len(){
        if input[i].r != output[i].r || input[i].g != output[i].g || input[i].b != output[i].b{
            println!("{} {}", "input red:", input[i].r);
            println!("{} {}", "output red:", output[i].r);
            println!("{} {}", "input green:", input[i].g);
            println!("{} {}", "output green:", output[i].g);
            println!("{} {}", "input blue:", input[i].b);
            println!("{} {}", "output blue:", output[i].b);
            println!();

            equal = false;
            if i > 100{
                break;
            }
        }
    }

    if equal{
        println!("YAY IT WORKS");
    }
    else{
        println!("DOESN'T WORK");
    }
}