use color_eyre::Result;
use game::camera::Camera;
use game::Game;
use log::info;
use network::init_player;
use network::tcp::TcpConn;
use packet::types::SessionInit;
use packet::Packet;
use raylib::ffi::TraceLogLevel;

mod game;
mod network;
mod packet;

#[tokio::main]
async fn main() -> Result<()> {
    info!("initializing window...");
    let (mut rl, thread) = raylib::init().size(700, 400).title("Rust Client").build();
    rl.set_trace_log(TraceLogLevel::LOG_WARNING);

    env_logger::init();
    let addr = "127.0.0.1:9000";

    info!("initializing tcp connection...");

    let mut tcp_conn = TcpConn::new(addr).await?;
    let client_id = init_player("Simon", &mut tcp_conn).await?;
    info!("initialized play with id {:#?}", client_id);

    let token = Some("gmCOC9");
    // let session_init_request = Packet::<SessionInit>::new(client_id, None)?;
    let session_init_request = Packet::<SessionInit>::new(client_id, token)?;
    tcp_conn.send_packet(&session_init_request).await?;

    // return Ok(());

    let camera = Camera::init(&rl);
    Game::init("Simon", client_id, tcp_conn, &mut rl).run(rl, thread, camera);

    Ok(())
}
