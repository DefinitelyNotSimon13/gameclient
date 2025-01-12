use thiserror::Error;
use uuid::Uuid;

use crate::packet::{flags::Flags, Packet, SessionInit};

impl Packet<SessionInit> {
    pub fn new(client_id: Uuid, token: Option<&str>) -> Result<Self, SessionInitError> {
        let payload_len = match token {
            Some(token) => 6,
            None => 0,
        };

        if let Some(token) = token {
            if token.len() != 6 {
                return Err(SessionInitError::InvalidTokenLength);
            }
        }

        Ok(Self {
            packet_type: SessionInit,
            client_id,
            flags: Flags::empty(),
            payload_len,
            payload: token.map_or_else(Vec::new, |s| s.as_bytes().to_vec()),
        })
    }
}

#[derive(Debug, Error)]
pub enum SessionInitError {
    #[error("invalid session token length")]
    InvalidTokenLength,
}
