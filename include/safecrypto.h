/*****************************************************************************
 * Copyright (C) Queen's University Belfast, ECIT, 2016                      *
 *                                                                           *
 * This file is part of libsafecrypto.                                       *
 *                                                                           *
 * This file is subject to the terms and conditions defined in the file      *
 * 'LICENSE', which is part of this source code package.                     *
 *****************************************************************************/

/**
 * @file safecrypto.h
 * @author n.smyth@qub.ac.uk
 * @date 10 Aug 2016
 * @brief Header file for the SAFEcrypto library.
 *
 * This header file and safecrypto_types.h are the only header files
 * that must be distributed with a pre-built library.
 *
 * Git commit information:
 *   Author: $SC_AUTHOR$
 *   Date:   $SC_DATE$
 *   Branch: $SC_BRANCH$
 *   Id:     $SC_IDENT$
 */

#pragma once

#include "safecrypto_types.h"


#ifdef __cplusplus
extern "C" {
#endif

/// Definitions for the maximum permited length of a string
#define SC_MAX_NAME_LEN       64

/// The maximum permitted length of an IBE user ID byte array
#define SC_IBE_MAX_ID_LENGTH  128


/// An enumerated type for print debug levels
typedef enum sc_debug_level {
    SC_LEVEL_NONE = 0,
    SC_LEVEL_ERROR,
    SC_LEVEL_WARNING,
    SC_LEVEL_INFO,
    SC_LEVEL_DEBUG,
} sc_debug_level_e;


/// A list of the lossless compression coding types to be used
#define ENTROPY_LIST(m) \
   m(SC_ENTROPY_NONE) \
   m(SC_ENTROPY_BAC) \
   m(SC_ENTROPY_BAC_RLE) \
   m(SC_ENTROPY_HUFFMAN_STATIC) \
   m(SC_ENTROPY_STRONGSWAN)

/// An enumerated type for the choice of entropy coding scheme
GENERATE_ENUM(sc_entropy_type_e, ENTROPY_LIST, SC_ENTROPY_SCHEME_MAX);

/// A list of the enumerated types in the form of human readable strings
__attribute__((unused))
GENERATE_ENUM_NAMES(sc_entropy_names, ENTROPY_LIST, SC_ENTROPY_SCHEME_MAX);


/// A list of the available schemes
#define SCHEME_LIST(m) \
   m(SC_SCHEME_NONE) \
   m(SC_SCHEME_SIG_HELLO_WORLD) \
   m(SC_SCHEME_SIG_BLISS) \
   m(SC_SCHEME_SIG_RING_TESLA) \
   m(SC_SCHEME_ENC_RLWE) \
   m(SC_SCHEME_KEM_ENS) \
   m(SC_SCHEME_SIG_ENS) \
   m(SC_SCHEME_SIG_ENS_WITH_RECOVERY) \
   m(SC_SCHEME_IBE_DLP) \
   m(SC_SCHEME_SIG_DLP) \
   m(SC_SCHEME_SIG_DLP_WITH_RECOVERY) \
   m(SC_SCHEME_SIG_DILITHIUM) \
   m(SC_SCHEME_SIG_DILITHIUM_G) \
   m(SC_SCHEME_KEM_KYBER) \
   m(SC_SCHEME_ENC_KYBER_CPA) \
   m(SC_SCHEME_ENC_KYBER_HYBRID)

/// An enumerated type for the choice of scheme
GENERATE_ENUM(sc_scheme_e, SCHEME_LIST, SC_SCHEME_MAX);

/// A list of the enumerated types in the form of human readable strings
__attribute__((unused))
GENERATE_ENUM_NAMES(sc_scheme_names, SCHEME_LIST, SC_SCHEME_MAX);

/// A struct used to store the coding details for produced data
SC_STRUCT_PACK_START
typedef struct _sc_stat_coding {
    size_t bits;
    size_t bits_coded;
    char   name[32];
} SC_STRUCT_PACKED sc_stat_coding_t;
SC_STRUCT_PACK_END

/// The types of data produced by the SAFEcrypto library
typedef enum _sc_stat_component {
    SC_STAT_PUB_KEY = 0,
    SC_STAT_PRIV_KEY,
    SC_STAT_SIGNATURE,
    SC_STAT_EXTRACT,
    SC_STAT_ENCRYPT,
    SC_STAT_ENCAPSULATE,
} sc_stat_component_e;

/// A struct used to store statistics for the algorithms
SC_STRUCT_PACK_START
typedef struct _sc_statistics {
    sc_scheme_e scheme;
    size_t param_set;
    size_t keygen_num;
    size_t keygen_num_trials;
    size_t pub_keys_encoded;
    size_t pub_keys_loaded;
    size_t priv_keys_encoded;
    size_t priv_keys_loaded;
    size_t sig_num;
    size_t sig_num_trials;
    size_t sig_num_verified;
    size_t sig_num_unverified;
    size_t encrypt_num;
    size_t decrypt_num;
    size_t encapsulate_num;
    size_t decapsulate_num;
    size_t extract_num;
    size_t extract_keys_loaded;
    size_t num_components[6];
#if 0
    sc_stat_coding_t *components[6];
#else
    sc_stat_coding_t components[6][5];
#endif
} SC_STRUCT_PACKED sc_statistics_t;
SC_STRUCT_PACK_END


/// @todo Add error checking for incompatible flags, e.g. Ziggurat
/// and Bernoulli are selected

/// A flag bit used to indicate that a further 32-bit word of
/// configuration flags will follow
#define SC_FLAG_MORE                    0x80000000

#define SC_FLAG_NONE                    0x00000000  ///< Disable all flags

/// A list of the flags associated with optional features
/// Word 0:
/// @{
#define SC_FLAG_0_ENTROPY_BAC                0x00000001  ///< BAC compression
#define SC_FLAG_0_ENTROPY_BAC_RLE            0x00000002  ///< BAC with RLE compression
#define SC_FLAG_0_ENTROPY_STRONGSWAN         0x00000004  ///< strongSwan compatible Huffman compression
#define SC_FLAG_0_ENTROPY_HUFFMAN_STATIC     0x00000008  ///< Huffman compression
#define SC_FLAG_0_SAMPLE_PREC_MASK           0x00000070  ///< A mask used to isloate the Gaussian sample precision bits
#define SC_FLAG_0_SAMPLE_DEFAULT             0x00000000  ///< Use default Gaussian sampling precision
#define SC_FLAG_0_SAMPLE_32BIT               0x00000010  ///< Use 32-bit Gaussian sampling precision
#define SC_FLAG_0_SAMPLE_64BIT               0x00000020  ///< Use 64-bit Gaussian sampling precision
#define SC_FLAG_0_SAMPLE_128BIT              0x00000030  ///< Use 128-bit Gaussian sampling precision
#define SC_FLAG_0_SAMPLE_192BIT              0x00000040  ///< Use 192-bit Gaussian sampling precision
#define SC_FLAG_0_SAMPLE_256BIT              0x00000050  ///< Use 256-bit Gaussian sampling precision
#define SC_FLAG_0_SAMPLE_BLINDING            0x00000100  ///< Enable blinding countermeasures
#define SC_FLAG_0_SAMPLE_CDF                 0x00000200  ///< CDF Gaussian sampler
#define SC_FLAG_0_SAMPLE_KNUTH_YAO           0x00000400  ///< Knuth Yao Gaussian sampler
#define SC_FLAG_0_SAMPLE_ZIGGURAT            0x00000800  ///< Ziggurat gaussian sampler
#define SC_FLAG_0_SAMPLE_BAC                 0x00001000  ///< BAC Gaussian sampler
#define SC_FLAG_0_SAMPLE_HUFFMAN             0x00002000  ///< Huffman decoder Gaussian sampler
#define SC_FLAG_0_SAMPLE_BERNOULLI           0x00004000  ///< Bernoulli Gaussian sampler
#define SC_FLAG_0_HASH_LENGTH_MASK           0x00030000  ///< Mask used to isolate hash length selection
#define SC_FLAG_0_HASH_LENGTH_512            0x00000000  ///< Enable 512-bit hash
#define SC_FLAG_0_HASH_LENGTH_384            0x00010000  ///< Enable 384-bit hash
#define SC_FLAG_0_HASH_LENGTH_256            0x00020000  ///< Enable 256-bit hash
#define SC_FLAG_0_HASH_LENGTH_224            0x00030000  ///< Enable 224-bit hash
#define SC_FLAG_0_HASH_FUNCTION_MASK         0x001C0000  ///< Mask used to isloate hash algorithm
#define SC_FLAG_0_HASH_FUNCTION_DEFAULT      0x00000000  ///< Enable scheme default hash
#define SC_FLAG_0_HASH_BLAKE2                0x00040000  ///< Enable BLAKE2-B hash
#define SC_FLAG_0_HASH_SHA2                  0x00080000  ///< Enable SHA-2 hash
#define SC_FLAG_0_HASH_SHA3                  0x000C0000  ///< Enable SHA-3 hash
#define SC_FLAG_0_HASH_WHIRLPOOL             0x00100000  ///< Enable Whirlpool hash
#define SC_FLAG_0_REDUCTION_MASK             0x00E00000  ///< Mask used to isolate reduction selection
#define SC_FLAG_0_REDUCTION_REFERENCE        0x00200000  ///< Use reference arithmetic for reduction
#define SC_FLAG_0_REDUCTION_BARRETT          0x00400000  ///< Use Barrett reduction
#define SC_FLAG_0_REDUCTION_FP               0x00600000  ///< Use Floating Point reduction
#define SC_FLAG_0_THREADING_MASK             0x7C000000  ///< Mask used to identify the multithreading selection
#define SC_FLAG_0_THREADING_KEYGEN           0x04000000  ///< Enable multithreading support for key generation
#define SC_FLAG_0_THREADING_ENC_SIGN         0x08000000  ///< Enable multithreading support for encryption, signing, etc.
#define SC_FLAG_0_THREADING_DEC_VERIFY       0x10000000  ///< Enable multithreading support for decryption, verification, etc.
/// @}

/// Word 1:
/// @{
#define SC_FLAG_1_CSPRNG_AES_CTR_DRBG        0x00000001  ///< Enable AES CTR-DRBG
#define SC_FLAG_1_CSPRNG_CHACHA              0x00000002  ///< Enable CHACHA20-CSPRNG
#define SC_FLAG_1_CSPRNG_SALSA               0x00000004  ///< Enable SALSA20-CSPRNG
#define SC_FLAG_1_CSPRNG_ISAAC               0x00000008  ///< Enable ISAAC CSPRNG
#define SC_FLAG_1_CSPRNG_KISS                0x00000010  ///< Enable Keep It Simple Stupid PRNG
#define SC_FLAG_1_CSPRNG_AES_CTR             0x00000020  ///< Enable AES CTR mode
#define SC_FLAG_1_CSPRNG_SHA3_512_DRBG       0x00000100  ///< Enable SHA3-512 HASH-DRBG
#define SC_FLAG_1_CSPRNG_SHA3_256_DRBG       0x00000400  ///< Enable SHA3-256 HASH-DRBG
#define SC_FLAG_1_CSPRNG_SHA2_512_DRBG       0x00001000  ///< Enable SHA2-512 HASH-DRBG
#define SC_FLAG_1_CSPRNG_SHA2_256_DRBG       0x00004000  ///< Enable SHA2-256 HASH-DRBG
#define SC_FLAG_1_CSPRNG_BLAKE2_512_DRBG     0x00010000  ///< Enable BLAKE2-512 HASH-DRBG
#define SC_FLAG_1_CSPRNG_BLAKE2_256_DRBG     0x00040000  ///< Enable BLAKE2-256 HASH-DRBG
#define SC_FLAG_1_CSPRNG_WHIRLPOOL_DRBG      0x00100000  ///< Enable Whirlpool-512 HASH-DRBG
#define SC_FLAG_1_CSPRNG_USE_DEV_RANDOM      0x01000000  ///< Use /dev/random as an entropy source
#define SC_FLAG_1_CSPRNG_USE_DEV_URANDOM     0x02000000  ///< Use /dev/urandom as an entropy source
#define SC_FLAG_1_CSPRNG_USE_OS_RANDOM       0x04000000  ///< Use the OS random function as an entropy source
#define SC_FLAG_1_CSPRNG_USE_CALLBACK_RANDOM 0x08000000  ///< Use a callback function as an entropy source
/// @}

/// Word 2:
/// @{
#define SC_FLAG_2_MEMORY_TEMP_EXTERNAL       0x10000000  ///< Use an external memory array to store intermediate data
/// @}


/// The entropy coder configuration
typedef struct sc_entropy {
    sc_entropy_type_e type;
    void* entropy_coder;
} sc_entropy_t;


/// Forward declaration of the SAFEcrypto struct (user does not require a definition)
typedef struct _safecrypto safecrypto_t;


/** @name Library version
 *  Functions used to provide the library version.
 */
/**@{*/
/** @brief Retrieve the version number of the SAFEcrypto library.
 *
 *  @return A 32-bit version number
 */
extern UINT32 safecrypto_get_version(void);

/** @brief Retrieve the version number of the SAFEcrypto library s a human readable string.
 * 
 *  @return A string representing the SAFEcrypto version number
 */
extern char *safecrypto_get_version_string(void);
/**@}*/


/** @name Creation
 *  Functions used to create and destroy instances of the SAFEcrypto library.
 */
/**@{*/
/** @brief Create a SAFEcrypto object.
 *
 *  @param scheme The algorithm to be instantiated
 *  @param set The parameter set id
 *  @param flags An array of configuration options for the selected algorithm
 *  @return A pointer to a safecrypto object
 */
extern safecrypto_t *safecrypto_create(sc_scheme_e scheme, SINT32 set,
    const UINT32 *flags);

/** @brief Destroy a SAFEcrypto object
 *  @param sc A pointer to a safecrypto object
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_destroy(safecrypto_t *sc);
/**@}*/

/** @name Define external memory used for intermediate storage
 *  A function used to set a pointer to memory used for intermediate
 *  memory that is zeroed and discarded after any calls to the API.
 */
/**@{*/
/** @brief Set an intermediate memory pointer
 *  @param sc A pointer to a safecrypto object
 *  @param mem A pointer to a memory array
 *  @param len The size (in bytes) of the memory array (aligned to SC_DEFAULT_ALIGNED)
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_scratch_external(safecrypto_t *sc, void *mem, size_t len);

/** @name Obtain the size of the intermediate memory
 *  Returns the required size of the intermediate memory buffer.
 */
/**@{*/
/** @brief Set an intermediate memory pointer
 *  @param sc A pointer to a safecrypto object
 *  @param len A pointer to the size (in bytes) of the memory array
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_scratch_size(safecrypto_t *sc, size_t *len);

/** @name External entropy callback
 *  A function used to set an external function as an entropy source.
 */
/**@{*/
/** @brief Set a callback function pointer
 *  @param sc A pointer to a func_get_random_entropy function
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_entropy_callback(safecrypto_entropy_cb_func fn_entropy);
/**@}*/

/** @name Debug level
 *  Functions used to control the debug level of the SAFEcrypto library.
 */
/**@{*/
/** @brief Set the debug verbosity.
 *
 *  @param level The debug verboseness
 *  @param sc A pointer to a safecrypto object
 *  @param level The user specified debug verbosity
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_set_debug_level(safecrypto_t *sc, sc_debug_level_e level);

/** @brief Get the debug verbosity.
 *
 *  @param sc A pointer to a safecrypto object
 *  @return The enumerated debug level currently in operation
 */
extern sc_debug_level_e safecrypto_get_debug_level(safecrypto_t *sc);
/**@}*/

/** @name Error handling
 *  Functions associated with accessing and manipulating the error queue.
 */
/**@{*/
/// Obtain the error code from the error queue and erase it from the queue.
extern UINT32 safecrypto_err_get_error(safecrypto_t *sc);

/// Obtain the error code from the error queue, doesn't modify the queue.
extern UINT32 safecrypto_err_peek_error(safecrypto_t *sc);

/// Same as safecrypto_err_get_error(), but retrieves the file name and line number.
extern UINT32 safecrypto_err_get_error_line(safecrypto_t *sc,
    const char **file, SINT32 *line);

/// Same as safecrypto_err_peek_error(), but retrieves the file name and line number.
extern UINT32 safecrypto_err_peek_error_line(safecrypto_t *sc,
    const char **file, SINT32 *line);

/// Removes all error messages from the queue.
extern void safecrypto_err_clear_error(safecrypto_t *sc);
/**@}*/

/** @name Key Generation
 *  Functions used to populate the key pair.
 */
/**@{*/
/** @brief Create a SAFEcrypto key-pair and store it in the SAFEcrypto struct.
 *
 *  @param sc A pointer to a safecrypto object
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_keygen(safecrypto_t *sc);

/** @brief Configure the entropy coding tools to be used to compress the key-pair.
 *
 *  @param sc A pointer to a safecrypto object
 *  @param pub Entropy coding to be applied to the public key
 *  @param priv Entropy coding to be applied to the private key
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_set_key_coding(safecrypto_t *sc, sc_entropy_type_e pub,
    sc_entropy_type_e priv);

/** @brief Get the entropy coding tools to be used to compress the key-pair.
 *
 *  @param sc A pointer to a safecrypto object
 *  @param pub Entropy coding to be applied to the public key
 *  @param priv Entropy coding to be applied to the private key polynomial
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_get_key_coding(safecrypto_t *sc, sc_entropy_type_e *pub,
    sc_entropy_type_e *priv);

/** @brief Store a public key in the SAFEcrypto struct.
 *
 *  @param sc A pointer to a safecrypto object
 *  @param key A serialized public key
 *  @param key_len The length of the key array
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_public_key_load(safecrypto_t *sc, const UINT8 *key, size_t keylen);

/** @brief Store a private key in the SAFEcrypto struct.
 *
 *  @param sc A pointer to a safecrypto object
 *  @param key A serialized private key
 *  @param key_len The length of the key array
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_private_key_load(safecrypto_t *sc, const UINT8 *key, size_t keylen);

/** @brief Return the SAFEcrypto public key in a packed byte array.
 *
 *  @param sc A pointer to a safecrypto object
 *  @param key A serialized public key
 *  @param keylen The size of the returned public key
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_public_key_encode(safecrypto_t *sc, UINT8 **key, size_t *keylen);

/** @brief Return the SAFEcrypto private key in a packed byte array.
 *
 *  @param sc A pointer to a safecrypto object
 *  @param key A serialized private key
 *  @param keylen The size of the returned private key
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_private_key_encode(safecrypto_t *sc, UINT8 **key, size_t *keylen);

/**@}*/

/** @name Cryptographic Processing
 *  Functions associated with performing cryptographic processing.
 */
/**@{*/
/** @brief Use public-key to generate ciphertext and a master key.
 *
 *  @param safecrypto Object containing key pair and lattice parameters
 *  @param c Output ciphertext
 *  @param c_len Ciphertext length
 *  @param k Output master key
 *  @param k_len Master key length
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_encapsulation(safecrypto_t *sc,
	  UINT8 **c, size_t *c_len,
	  UINT8 **k, size_t *k_len);

/** @brief Use private-key and ciphertext to generate the master key.
 *
 *  @param sc Object containing key pair and lattice parameters
 *  @param c Input ciphertext
 *  @param c_len Ciphertext length
 *  @param k Output master key
 *  @param k_len Master key length
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_decapsulation(safecrypto_t *sc,
	  const UINT8 *c, size_t c_len,
	  UINT8 **k, size_t *k_len);

/** @brief IBE Set User Secret Key.
 *
 *  @param sc Object containing key pair and lattice parameters
 *  @param sklen The size of the user secret key in bytes
 *  @param sk The user secret key obtained from the Private Key Generator
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_secret_key(safecrypto_t *sc, size_t sklen, const UINT8 *sk);

/** @brief IBE Extract.
 *
 *  @param sc Object containing key pair and lattice parameters
 *  @param idlen The size of the User ID in bytes
 *  @param id The User ID
 *  @param sklen The size of the user secret key in bytes
 *  @param sk The output user secret key
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_ibe_extract(safecrypto_t *sc, size_t idlen, const UINT8 *id,
    size_t *sklen, UINT8 **sk);

/** @brief IBE public key encryption.
 *
 *  @param sc Object containing key pair and lattice parameters
 *  @param idlen The size of the receipient User ID in bytes
 *  @param id The receipient's User ID
 *  @param flen The size of the from array in bytes
 *  @param from The input message
 *  @param tlen The size of the ciphertext array in bytes
 *  @param to The output ciphertext
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_ibe_public_encrypt(safecrypto_t *sc,
    size_t idlen, const UINT8 *id,
    size_t flen, const UINT8 *from,
    size_t *tlen, UINT8 **to);

/** @brief PKE encryption.
 *
 *  @param sc Object containing key pair and lattice parameters
 *  @param flen The size of the from array in bytes
 *  @param from The input message
 *  @param tlen The size of the ciphertext array in bytes
 *  @param to The output ciphertext
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_public_encrypt(safecrypto_t *sc,
    size_t flen, const UINT8 *from, size_t *tlen, UINT8 **to);

/** @brief PKE decryption.
 *
 *  @param sc Object containing key pair and lattice parameters
 *  @param flen The size of the from array in bytes
 *  @param from The input ciphertext
 *  @param tlen The size of the output message in bytes
 *  @param to The output message
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_private_decrypt(safecrypto_t *sc,
    size_t flen, const UINT8 *from, size_t *tlen, UINT8 **to);

/** @brief Signature.
 *
 *  @param sc Object containing key pair and lattice parameters
 *  @param m The input message
 *  @param mlen The size of the message array in bytes
 *  @param sigret The output signature
 *  @param siglen The size of the signature array in bytes
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_sign(safecrypto_t *sc, const UINT8 *m, size_t mlen,
    UINT8 **sigret, size_t *siglen);

/** @brief Signature Verification.
 *
 *  @param sc Object containing key pair and lattice parameters
 *  @param m The input message
 *  @param mlen The size of the message array in bytes
 *  @param sigbuf The input signature
 *  @param siglen The size of the signature array in bytes
 *  @return Returns 1 on successful validation
 */
extern SINT32 safecrypto_verify(safecrypto_t *sc, const UINT8 *m, size_t mlen,
    const UINT8 *sigbuf, size_t siglen);
/**@}*/

/** @brief Signature with Message Recovery.
 *
 *  @param sc Object containing key pair and lattice parameters
 *  @param m The input message, modified upon return
 *  @param mlen The size of the message array in bytes when called, modified upon return
 *  @param sigret The output signature
 *  @param siglen The size of the signature array in bytes
 *  @return Returns 1 on success
 */
extern SINT32 safecrypto_sign_with_recovery(safecrypto_t *sc, UINT8 **m, size_t *mlen,
    UINT8 **sigret, size_t *siglen);

/** @brief Signature Verification with Message Recovery.
 *
 *  @param sc Object containing key pair and lattice parameters
 *  @param m The input message, modified upon return
 *  @param mlen The size of the message array in bytes, modified upon return
 *  @param sigbuf The input signature
 *  @param siglen The size of the signature array in bytes
 *  @return Returns 1 on successful validation
 */
extern SINT32 safecrypto_verify_with_recovery(safecrypto_t *sc, UINT8 **m, size_t *mlen,
    const UINT8 *sigbuf, size_t siglen);
/**@}*/

/** @brief Processing statistics in string form.
 *
 *  @param sc Object containing key pair and lattice parameters
 *  @return Returns a C string describing the previous crypto processing
 *          operation.
 */
extern const char * safecrypto_processing_stats(safecrypto_t *sc);

/** @brief Processing statistics.
 *
 *  @param sc Object containing key pair and lattice parameters
 *  @return Returns a pointer to an sc_statistics_t struct.
 */
extern const sc_statistics_t * safecrypto_get_stats(safecrypto_t *sc);


#ifdef __cplusplus
}
#endif

