use uuid::Uuid;

use crate::network::tcp::TcpConn;

pub struct GameState {
    username: String,
    client_id: Uuid,
    tcpConn: TcpConn,
    // udpConn: UdpConn,
}

impl GameState {
    pub fn init(username: &str, client_id: Uuid, tcpConn: TcpConn) -> Self {
        Self {
            username: username.to_string(),
            client_id,
            tcpConn,
        }
    }
}
