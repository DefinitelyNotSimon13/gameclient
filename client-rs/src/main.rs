use color_eyre::Result;
use log::info;
use network::init_player;
use network::tcp::TcpConn;
use raylib::prelude::*;

mod game;
mod network;
mod packet;

#[tokio::main]
async fn main() -> Result<()> {
    env_logger::init();
    let addr = "127.0.0.1:9000";

    info!("initializing tcp connection...");

    let mut tcp_conn = TcpConn::new(addr).await?;
    let client_id = init_player("Simon", &mut tcp_conn).await?;
    info!("initialized play with id {:#?}", client_id);

    loop {}

    return Ok(());

    let (mut rl, thread) = raylib::init().size(640, 480).title("Hello, World").build();

    while !rl.window_should_close() {
        let mut d = rl.begin_drawing(&thread);

        d.clear_background(Color::WHITE);
        d.draw_text("Hello, world!", 12, 12, 20, Color::BLACK);
    }

    Ok(())
}
