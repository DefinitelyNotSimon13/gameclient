use color_eyre::Result;
use game::camera::Camera;
use game::environment::draw_environment;
use game::player::{LocalPlayer, Player};
use log::info;
use network::init_player;
use network::tcp::TcpConn;
use raylib::consts::CameraMode;
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

    let (mut rl, thread) = raylib::init().size(640, 480).title("Hello, World").build();
    rl.set_target_fps(60);
    let camera = Camera::init(&rl);
    let mut player = LocalPlayer::new(Player {
        postition: Vector3::new(0.0, 1.0, 0.0),
        primary_color: Color::PURPLE,
        secondary_color: Color::DARKPURPLE,
    });

    // WARNING MoveSpeed is based on FPS - not so good
    while !rl.window_should_close() {
        player.update_position(&rl);

        let mut d = rl.begin_drawing(&thread);
        d.clear_background(Color::WHITE);

        {
            let mut d_3d = d.begin_mode3D(camera.camera);

            draw_environment(&mut d_3d);
            player.draw(&mut d_3d);
        }

        d.draw_fps(5, 5);
    }

    Ok(())
}
