use uuid::Uuid;

use crate::network::tcp::TcpConn;

use super::player::LocalPlayer;

pub struct GameState {
    pub local_player: LocalPlayer,
    client_id: Uuid,
    tcpConn: TcpConn,
}

impl GameState {
    pub fn init(local_player: LocalPlayer, client_id: Uuid, tcpConn: TcpConn) -> Self {
        Self {
            local_player,
            client_id,
            tcpConn,
        }
    }
}
