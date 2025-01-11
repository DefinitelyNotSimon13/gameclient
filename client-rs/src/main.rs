use color_eyre::Result;
use raylib::prelude::*;
use std::io::prelude::*;
use std::net::{TcpStream, UdpSocket};

fn main() -> Result<()> {
    let mut stream = TcpStream::connect("127.0.0.1:9000")?;
    let mut socket = UdpSocket::bind("[::]:0")?;

    let mut buf = [0; 10];
    let (amt, src) = socket.recv_From

    let mut buf = [0; 10];
    socket.send_to(&buf, "127.0.0.1:9000")?;

    stream.write(&[1])?;
    stream.read(&mut [0; 128])?;

    let (mut rl, thread) = raylib::init().size(640, 480).title("Hello, World").build();

    while !rl.window_should_close() {
        let mut d = rl.begin_drawing(&thread);

        d.clear_background(Color::WHITE);
        d.draw_text("Hello, world!", 12, 12, 20, Color::BLACK);
    }

    Ok(())
}
