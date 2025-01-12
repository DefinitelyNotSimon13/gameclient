use flags::Flags;
use types::{PacketTypeState, PlayerInit, SessionInit};
use thiserror::Error;
use uuid;
use uuid::Uuid;
use version::VersionError;

pub mod flags;
pub mod from_bytes;
pub mod to_bytes;
pub mod types;
pub mod version;

pub const PACKET_VERSION: u8 = 1;

#[derive(Debug)]
pub struct Packet<T: PacketTypeState> {
    pub packet_type: T,
    pub client_id: Uuid,
    pub flags: Flags,
    pub payload_len: u32,
    pub payload: Vec<u8>,
}

impl<T: PacketTypeState> Packet<T> {}

pub trait ToBytes {
    fn to_bytes(&self) -> Vec<u8>;
}

pub trait ToByte {
    fn to_byte(&self) -> u8;
}

pub trait FromBytes {
    type Error;

    fn from_bytes(data: &[u8]) -> Result<Self, Self::Error>
    where
        Self: Sized;
}

pub trait FromByte {
    type Error;

    fn from_byte(byte: u8) -> Result<Self, Self::Error>
    where
        Self: Sized;
}

impl ToBytes for u32 {
    fn to_bytes(&self) -> Vec<u8> {
        self.to_le_bytes().to_vec()
    }
}

#[derive(Debug, Error)]
pub enum PacketError {
    #[error("unknown version byte: {0}")]
    UnknownVersion(u8),

    #[error("invalid packet type. Expected {expected}, got {found}")]
    InvalidPacketType { expected: u8, found: u8 },

    #[error("not enough bytes to parse the header fields")]
    NotEnoughHeaderBytes,

    #[error("not enough bytes to parse the payload")]
    NotEnoughPayloadBytes,

    #[error("to many bytes provided. Expected {expected}, got {found}")]
    ToManyBytes { expected: usize, found: usize },

    #[error("invalid flags byte")]
    FlagsError,

    #[error("uuid parse error")]
    UuidError(#[from] uuid::Error),

    #[error("version parse error")]
    VersionErr(#[from] VersionError),
}
