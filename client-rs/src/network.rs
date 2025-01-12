use log::info;
use tcp::TcpConn;
use thiserror::Error;
use uuid::Uuid;

use crate::packet::{
    types::PlayerInit,
    FromBytes, Packet, PacketError,
};
use crate::packet::types::player_init::{PlayerInitError, PLAYER_INIT_RESPONSE_SIZE};

pub mod tcp;
pub mod udp;

pub async fn init_player(username: &str, tcp_conn: &mut TcpConn) -> Result<Uuid, NetworkError> {
    info!("initializing player...");
    let player_init_request = Packet::<PlayerInit>::new(username)?;
    tcp_conn.send_packet(&player_init_request).await?;
    let mut response_buf = [0; PLAYER_INIT_RESPONSE_SIZE];

    tcp_conn.wait_for_packet(&mut response_buf).await?;

    Ok(Packet::<PlayerInit>::from_bytes(&response_buf)?.get_client_id()?)
}

#[derive(Debug, Error)]
pub enum NetworkError {
    #[error("failed to create player init packet")]
    PlayerInitFailure(#[from] PlayerInitError),

    #[error("failed to create packet")]
    PacketInitFailure(#[from] PacketError),

    #[error("failed to connect to server")]
    FailedToConnect { e: std::io::Error },

    #[error("failed to write packet")]
    FailedToWrite { e: std::io::Error },

    #[error("failed to retrieve packet")]
    FailedToRetrieve { e: std::io::Error },

    #[error("no bytes read")]
    NoBytesRead,
}
