/*****************************************************************************
 * Copyright (C) Queen's University Belfast, ECIT, 2016                      *
 *                                                                           *
 * This file is part of libsafecrypto.                                       *
 *                                                                           *
 * This file is subject to the terms and conditions defined in the file      *
 * 'LICENSE', which is part of this source code package.                     *
 *****************************************************************************/

/**
 * @file
 * These variables, functions and preprocessor definitions and macros are
 * not to be exposed to the user. This header file should not be
 * distributed with a pre-built library.
 *
 * @author n.smyth@qub.ac.uk
 * @date 10 Aug 2016
 * @brief Private functions and variables that are not exposed to the user.
 *
 * Git commit information:
 *   Author: $SC_AUTHOR$
 *   Date:   $SC_DATE$
 *   Branch: $SC_BRANCH$
 *   Id:     $SC_IDENT$
 */


#pragma once

#include <stdlib.h>
#include "safecrypto.h"


// See http://nadeausoftware.com/articles/2012/10/c_c_tip_how_detect_compiler_name_and_version_using_compiler_predefined_macros

/// Define a macro for detection of GNU GCC
#define GNU_GCC_COMPILER (defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER))


/// Definitions for the maximum permited length of a filename
#define SC_MAX_FILENAME_LEN   128

/// The maximum number of entries in the algorithm table
#define ALG_TABLE_MAX   16

/// The maximum number of distributions for lossless compression
#define ENTROPY_MAX_DIST     8


// Globally define the reduction method
#ifdef HAVE_AVX2
#ifndef NEEDS_NTT_AVX
#define NEEDS_NTT_AVX
#endif
#else
#ifndef NEEDS_NTT_FLOATING_POINT
#define NEEDS_NTT_FLOATING_POINT
#endif
#endif


#if defined(CONSTRAINED_RAM) || defined(CONSTRAINED_ROM) || defined(CONSTRAINED_CPU)
#define CONSTRAINED_SYSTEM
#endif

/// @name Helper functions for memory allocation and string manipulation
/**@{*/
extern void * sc_malloc(size_t size);
extern void * sc_realloc(void *ptr, size_t size);
extern void sc_free(void *ptr, size_t size);
extern SINT32 sc_mem_is_zero(volatile const UINT8 *a, size_t n);
extern void sc_explicit_memzero(void * const ptr, const size_t size);
extern void * sc_memcpy(void *dest, const void *src, size_t size);
extern char * sc_strcpy(char *dest, const char *src, size_t dest_size);
extern void sc_free_stats(safecrypto_t *sc);
extern SINT32 sc_init_stats(safecrypto_t *sc, size_t pub_key, size_t priv_key,
    size_t signature, size_t extract, size_t encrypt, size_t encapsulate);

#define SC_MALLOC(s)        sc_malloc(s)
#define SC_REALLOC(s,l)     sc_realloc(s,l)
#define SC_FREE(s,l)        sc_free((s), (l)); (s) = NULL;
#define SC_MEMZERO(s,l)     sc_explicit_memzero((s), (l))
#define SC_MEMCOPY(d,s,l)   sc_memcpy((d), (s), (l))
#define SC_MEM_IS_ZERO(s,l) sc_mem_is_zero((s), (l))
/**@}*/


/// Endianness conversion
/// @{
#ifdef HAVE_64BIT
void sc_swap_copy_64(void* to, size_t index, const void* from, size_t length);
#endif
void sc_swap_copy_32(void* to, size_t index, const void* from, size_t length);

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
// We are big-endian
#define SC_LITTLE_ENDIAN_32(x) ((((x) & 0xFF000000) >> 24) | \
                                (((x) & 0x00FF0000) >>  8) | \
                                (((x) & 0x0000FF00) <<  8) | \
                                (((x) & 0x000000FF) << 24))
#define SC_BIG_ENDIAN_32(x)    (x)
#define SC_LITTLE_ENDIAN_32_COPY(to, index, from, length) \
            sc_swap_copy_32((to), (index), (from), (length))
#define SC_BIG_ENDIAN_32_COPY(to, index, from, length) \
            SC_MEMCOPY((to) + (index), (from), (length))
#ifdef HAVE_64BIT
#define SC_LITTLE_ENDIAN_64(x) ((((x) & 0xFF00000000000000) >> 56) | \
                                (((x) & 0x00FF000000000000) >> 40) | \
                                (((x) & 0x0000FF0000000000) >> 24) | \
                                (((x) & 0x000000FF00000000) >>  8) | \
                                (((x) & 0x00000000FF000000) <<  8) | \
                                (((x) & 0x0000000000FF0000) << 24) | \
                                (((x) & 0x000000000000FF00) << 40) | \
                                (((x) & 0x00000000000000FF) << 56))
#define SC_BIG_ENDIAN_64(x)    (x)
#define SC_LITTLE_ENDIAN_64_COPY(to, index, from, length) \
            sc_swap_copy_64((to), (index), (from), (length))
#define SC_BIG_ENDIAN_64_COPY(to, index, from, length) \
            SC_MEMCOPY((to) + (index), (from), (length))
#endif
#else
// We are little-endian
#define SC_LITTLE_ENDIAN_32(x) (x)
#define SC_BIG_ENDIAN_32(x)    ((((x) & 0xFF000000) >> 24) | \
                                (((x) & 0x00FF0000) >>  8) | \
                                (((x) & 0x0000FF00) <<  8) | \
                                (((x) & 0x000000FF) << 24))
#define SC_LITTLE_ENDIAN_32_COPY(to, index, from, length) \
            SC_MEMCOPY((to) + (index), (from), (length))
#define SC_BIG_ENDIAN_32_COPY(to, index, from, length) \
            sc_swap_copy_32((to), (index), (from), (length))
#ifdef HAVE_64BIT
#define SC_LITTLE_ENDIAN_64(x) (x)
#define SC_BIG_ENDIAN_64(x)    ((((x) & 0xFF00000000000000) >> 56) | \
                                (((x) & 0x00FF000000000000) >> 40) | \
                                (((x) & 0x0000FF0000000000) >> 24) | \
                                (((x) & 0x000000FF00000000) >>  8) | \
                                (((x) & 0x00000000FF000000) <<  8) | \
                                (((x) & 0x0000000000FF0000) << 24) | \
                                (((x) & 0x000000000000FF00) << 40) | \
                                (((x) & 0x00000000000000FF) << 56))
#define SC_LITTLE_ENDIAN_64_COPY(to, index, from, length) \
            SC_MEMCOPY((to) + (index), (from), (length))
#define SC_BIG_ENDIAN_64_COPY(to, index, from, length) \
            sc_swap_copy_64((to), (index), (from), (length))
#endif
#endif
/**@}*/



/// An enumerated type describing the blinding techniques that are
/// availble for the sampling schemes
typedef enum sample_blinding {
    NORMAL_SAMPLES = 0,
    BLINDING_SAMPLES,
    SHUFFLE_SAMPLES,
} sample_blinding_e;

/// Pattern masking techniques
/// @{
#define SCA_PATTERN_DISABLE               0x00000000   ///< Disable all masking mechanisms
#define SCA_PATTERN_SAMPLE_CACHE_ACCESS   0x00000001   ///< Enable random cache access of any Gaussian sample LUT
#define SCA_PATTERN_SAMPLE_DISCARD_MASK   0x00000006
#define SCA_PATTERN_SAMPLE_DISCARD_LO     0x00000002   ///< Enable discarding Gaussian samples at a low rate (6.25%)
#define SCA_PATTERN_SAMPLE_DISCARD_MD     0x00000004   ///< Enable discarding Gaussian samples at a medium rate (25%)
#define SCA_PATTERN_SAMPLE_DISCARD_HI     0x00000006   ///< Enable discarding Gaussian samples at a high rate (50%)
#define SCA_PATTERN_SAMPLE_NON_CT_MASK    0x00000008   ///< Enable the masking of non-constant time Gaussian sampling
#define SCA_PATTERN_SAMPLE_LUT_MOVE       0x00000010   ///< Enable the transfer of the Gaussian sample table(s) to a new address
                                                       ///  for every cryptographic operation (dynamic memory must be used)
/// @}

/// A list of the random sampling schemes that are available
#define SAMPLING_LIST(m) \
    m(CDF_GAUSSIAN_SAMPLING) \
    m(KNUTH_YAO_GAUSSIAN_SAMPLING) \
    m(BAC_GAUSSIAN_SAMPLING) \
    m(HUFFMAN_GAUSSIAN_SAMPLING) \
    m(ZIGGURAT_GAUSSIAN_SAMPLING) \
    m(BERNOULLI_GAUSSIAN_SAMPLING) \
    m(KNUTH_YAO_FAST_GAUSSIAN_SAMPLING)

/// An enumerated type describing the random sampling schemes that are available
GENERATE_ENUM(random_sampling_e, SAMPLING_LIST, SAMPLING_MAX);

/// A list of the random sampling enumerated types in the form of human readable strings
__attribute__((unused))
GENERATE_ENUM_NAMES(sc_sampler_names, SAMPLING_LIST, SAMPLING_MAX);



/// Check if the SAFEcrypto struct is valid
/// @return Returns 1 if successful, 0 otherwise
extern SINT32 check_safecrypto(safecrypto_t *sc);



/// A struct used to store the private-key
typedef struct sc_key sc_privkey_t;

/// A struct used to store the public-key
typedef struct sc_key sc_pubkey_t;

/// A struct used to store the serialized key
SC_STRUCT_PACK_START
struct sc_key {
    void *key;    ///< Key from key-pair
    size_t len;   ///< Length of the key
} SC_STRUCT_PACKED;
SC_STRUCT_PACK_END

// Forward declaration of the error queue struct
typedef struct err_ctrl err_ctrl_t;

// Forward declaration of the mutex struct
typedef struct _sc_mutex sc_mutex_t;

typedef struct bliss_cfg_t bliss_cfg_t;
typedef struct dilithium_cfg_t dilithium_cfg_t;
typedef struct ring_tesla_cfg_t ring_tesla_cfg_t;
typedef struct rlwe_enc_cfg_t rlwe_enc_cfg_t;
typedef struct ens_kem_cfg_t ens_kem_cfg_t;
typedef struct kyber_cfg_t kyber_cfg_t;
typedef struct ens_dlp_sig_cfg_t ens_dlp_sig_cfg_t;
typedef struct dlp_ibe_cfg_t dlp_ibe_cfg_t;
typedef struct _ec_cfg_t ec_cfg_t;

typedef struct _sc_stat_coding sc_stat_coding_t;
typedef enum _sc_stat_component sc_stat_component_e;
typedef struct _sc_statistics sc_statistics_t;

// Forward declaration of the Gaussian Sampler interface
typedef struct _utils_sampling utils_sampling_t;

/// An enumerated type describing sample blinding techniques
typedef enum sample_blinding sample_blinding_e;

// Forward declaration of the XOF interface
typedef struct _utils_crypto_xof utils_crypto_xof_t;
typedef struct _utils_arith_ntt utils_arith_ntt_t;
typedef struct _utils_arith_poly utils_arith_poly_t;
typedef struct _utils_arith_vec utils_arith_vec_t;
typedef struct _utils_crypto_hash utils_crypto_hash_t;


// Forward declaration of the PRNG state/context
typedef struct prng_ctx_t prng_ctx_t;

/// A struct used to store the parameters, private-key and public-key
SC_STRUCT_PACK_START
struct _safecrypto {
    void **sampler;                     ///< A pointer to a sampler of a specified distribution and type

    sc_statistics_t stats;              ///< A struct used to store statistics

    err_ctrl_t *error_queue;            ///< The error queue that logs runtime errors
    sc_debug_level_e debug_level;       ///< The debug level of the log file

    prng_ctx_t **prng_ctx;              ///< The PRNG context/state

    sc_privkey_t   *privkey;            ///< Pointer to a struct used to store the private-key
    sc_pubkey_t    *pubkey;             ///< Pointer to a struct used to store the Public-key

    /// @todo Place these in a union ...
    bliss_cfg_t *bliss;                 ///< BLISS configuration
    dilithium_cfg_t *dilithium;         ///< Dilithium configuration
    ring_tesla_cfg_t *ring_tesla;       ///< ring-TESLA configuration
    rlwe_enc_cfg_t *rlwe_enc;           ///< RLWE configuration
    ens_kem_cfg_t *ens_kem;             ///< ENS "The whole is ..." - NTRU KEM configuration
    kyber_cfg_t *kyber;                 ///< Kyber KEM configuration
    ens_dlp_sig_cfg_t *ens_dlp_sig;     ///< ENS/DLP - signature configuration
    dlp_ibe_cfg_t *dlp_ibe;             ///< ENS SAFEcrypto IBE configuration
    ec_cfg_t *ec;                       ///< ECDH and ECDSA configuration

    utils_sampling_t *sc_gauss;         ///< A Gaussian Sampler
    random_sampling_e sampling;         ///< The sampling scheme to be used
    UINT32 sampling_precision;          ///< Sampling precision (bits)
    sample_blinding_e blinding;         ///< Enable sample blinding or shuffling countermeasures (scheme independendt)
    UINT32 pattern;                     ///< A bitfield used to enable techniques to mask the pattern of algorithms and memory access

    void* dist[ENTROPY_MAX_DIST];       ///< Distributions used for lossless compression purposes
    size_t dist_n[ENTROPY_MAX_DIST];    ///< log base 2 of the size of the distribution

    const utils_arith_ntt_t  *sc_ntt;
    const utils_arith_poly_t *sc_poly;
    const utils_arith_vec_t  *sc_vec;

    SINT32 *temp;                       ///< A pointer to the scratch memory used for intermediate storage
    size_t temp_size;                   ///< The size of the intermediate storage memory
    SINT32 temp_external_flag;          ///< A flag indicating if the temp memory is externally provided
    SINT32 temp_ready;                  ///< A flag indicating if external temp memory has been defined

    utils_crypto_hash_t      *hash;
    utils_crypto_xof_t       *xof;      ///< A pointer to an instance of an Extensible Output Function

    sc_entropy_t coding_pub_key;        ///< The entropy coding to be applied to public key
    sc_entropy_t coding_priv_key;       ///< The entropy coding to be applied to the private key
    sc_entropy_t coding_user_key;       ///< The entropy coding to be applied to the User Secret Key
    sc_entropy_t coding_signature;      ///< The entropy coder associated with signatures
    sc_entropy_t coding_encryption;     ///< The entropy coder associated with encryption/decryption

    int alg_index;                      ///< The index of the algorithm within safecrypto_algorithms
    sc_scheme_e scheme;                 ///< The enumerated type of the scheme
} SC_STRUCT_PACKED;
SC_STRUCT_PACK_END


/// Algorithm creation function pointer
typedef SINT32 (*creation)(safecrypto_t *, SINT32, const UINT32*);

/// Function pointer used to free resources associated with an algorithm
typedef SINT32 (*destruction)(safecrypto_t *);

/// Key pair generation function pointer
typedef SINT32 (*keygeneration)(safecrypto_t *);

/// Public key load function pointer
typedef SINT32 (*public_key_load)(safecrypto_t *, const UINT8*, size_t);

/// Set key-pair lossless compression coding
typedef SINT32 (*setkeycoding)(safecrypto_t *, sc_entropy_type_e, sc_entropy_type_e);

/// Get key-pair lossless compression coding
typedef SINT32 (*getkeycoding)(safecrypto_t *, sc_entropy_type_e*, sc_entropy_type_e*);

/// Private key load function pointer
typedef SINT32 (*private_key_load)(safecrypto_t *, const UINT8*, size_t);

/// Public key encode function pointer
typedef SINT32 (*public_key_encode)(safecrypto_t *, UINT8**, size_t*);

/// Private key encode function pointer
typedef SINT32 (*private_key_encode)(safecrypto_t *, UINT8**, size_t*);

/// A function pointer for KEM encapsulation
typedef SINT32 (*kem_encap)(safecrypto_t *, UINT8**, size_t*,
    UINT8**, size_t*);

/// A function pointer for KEM decapsulation
typedef SINT32 (*kem_decap)(safecrypto_t *, const UINT8*, size_t,
    UINT8**, size_t*);

/// A function pointer to set the current user secret key
typedef SINT32 (*ibe_secret_key)(safecrypto_t *, size_t, const UINT8 *);

/// A function pointer for extraction of a secret user key
typedef SINT32 (*ibe_extract)(safecrypto_t *, size_t, const UINT8 *,
    size_t *, UINT8 **);

/// A function pointer for IBE encryption using the public key
typedef SINT32 (*ibe_public_encrypt)(safecrypto_t *, size_t, const UINT8 *,
    size_t, const UINT8 *,
    size_t *, UINT8 **);

/// A function pointer for encryption using the public key
typedef SINT32 (*public_encrypt)(safecrypto_t *, size_t, const UINT8 *,
    size_t *, UINT8 **);

/// A function pointer for decryption using the private key
typedef SINT32 (*private_decrypt)(safecrypto_t *, size_t, const UINT8 *,
    size_t *, UINT8 **);

/// A function pointer to create a signature
typedef SINT32 (*sign)(safecrypto_t *, const UINT8 *, size_t,
    UINT8 **, size_t *);

/// A function pointer to verify a signature
typedef SINT32 (*verify)(safecrypto_t *, const UINT8 *, size_t,
    const UINT8 *, size_t);

/// A function pointer to create a signature with message recovery
typedef SINT32 (*sign_recovery)(safecrypto_t *, UINT8 **, size_t *,
    UINT8 **, size_t *);

/// A function pointer to verify a signature with message recovery
typedef SINT32 (*verify_recovery)(safecrypto_t *, UINT8 **, size_t *,
    const UINT8 *, size_t);

/// A function pointer for DH public key generation
typedef SINT32 (*dh_public_init)(safecrypto_t *, size_t *, UINT8 **);

/// A function pointer for DH shared secret generation using an input public key
typedef SINT32 (*dh_secret_final)(safecrypto_t *, size_t, const UINT8 *,
    size_t *, UINT8 **);

/// A function pointer to obtain cumulative processing statistics
typedef char * (*stats)(safecrypto_t *);

/// A struct defining the function pointers associated with a named algorithm
SC_STRUCT_PACK_START
typedef struct _safecrypto_alg {
    sc_scheme_e scheme;
    creation           create;
    destruction        destroy;
    keygeneration      keygen;
    setkeycoding       set_key_coding;
    getkeycoding       get_key_coding;
    public_key_load    pubkey_load;
    private_key_load   privkey_load;
    public_key_encode  pubkey_encode;
    private_key_encode privkey_encode;
    kem_encap          encapsulation;
    kem_decap          decapsulation;
    ibe_secret_key     secret_key;
    ibe_extract        extract;
    ibe_public_encrypt ibe_encrypt;
    public_encrypt     encrypt;
    private_decrypt    decrypt;
    sign               signing;
    verify             verification;
    sign_recovery      signing_recovery;
    verify_recovery    verification_recovery;
    dh_public_init     dh_init;
    dh_secret_final    dh_final;
    stats              processing_stats;
} SC_STRUCT_PACKED safecrypto_alg_t;
SC_STRUCT_PACK_END

