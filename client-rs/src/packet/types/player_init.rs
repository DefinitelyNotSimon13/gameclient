use crate::packet::{Packet, PlayerInit};
use crate::packet::flags::Flags;
use thiserror::Error;
use uuid::Uuid;

pub const PLAYER_INIT_RESPONSE_SIZE: usize = 23;

impl Packet<PlayerInit> {
    pub fn new(username: &str) -> Result<Self, PlayerInitError> {
        let payload_len: u32 = username
            .len()
            .try_into()
            .map_err(|_| PlayerInitError::UsernameToLong)?;
        if payload_len < 4 {
            Err(PlayerInitError::UsernameToLong)?
        } else {
            Ok(Self {
                packet_type: PlayerInit,
                client_id: Uuid::nil(),
                flags: Flags::empty(),
                payload_len,
                payload: username.as_bytes().to_vec(),
            })
        }
    }

    pub fn get_client_id(&self) -> Result<Uuid, PlayerInitError> {
        if self.client_id == Uuid::nil() {
            Err(PlayerInitError::UuidNotAssigned)?
        } else if self.flags.contains(Flags::BIT0) {
            Err(PlayerInitError::ServerError)?
        } else {
            Ok(self.client_id)
        }
    }
}

#[derive(Debug, Error)]
pub enum PlayerInitError {
    #[error("the given username is to long")]
    UsernameToLong,

    #[error("username to short")]
    UserNameToShort,

    #[error("uuid was not assigned by server")]
    UuidNotAssigned,

    #[error("server error")]
    ServerError,
}
