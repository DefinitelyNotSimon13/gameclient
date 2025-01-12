use color_eyre::eyre::Result;
use thiserror::Error;

use super::{FromByte, ToByte};

#[derive(Debug)]
pub enum Version {
    V0,
    V1,
}

impl ToByte for Version {
    fn to_byte(&self) -> u8 {
        use Version::*;
        match self {
            V0 => 0_u8,
            V1 => 1_u8,
        }
    }
}

impl FromByte for Version {
    type Error = VersionError;
    fn from_byte(byte: u8) -> Result<Self, Self::Error> {
        use Version::*;
        match byte {
            0 => Ok(V0),
            1 => Ok(V1),
            _ => Err(VersionError::UnknownVersion),
        }
    }
}

#[derive(Error, Debug)]
pub enum VersionError {
    #[error("invalid version byte (expected len == 1, found {length:?})")]
    ByteSize { length: usize },

    #[error("unkown version")]
    UnknownVersion,

    #[error("deprecated version")]
    DeprecatedVersion,
}
