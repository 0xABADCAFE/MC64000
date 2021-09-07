/**
 *   888b     d888  .d8888b.   .d8888b.      d8888  888    d8P
 *   8888b   d8888 d88P  Y88b d88P  Y88b    d8P888  888   d8P
 *   88888b.d88888 888    888 888          d8P 888  888  d8P
 *   888Y88888P888 888        888d888b.   d8P  888  888d88K
 *   888 Y888P 888 888        888P "Y88b d88   888  8888888b
 *   888  Y8P  888 888    888 888    888 8888888888 888  Y88b
 *   888   "   888 Y88b  d88P Y88b  d88P       888  888   Y88b
 *   888       888  "Y8888P"   "Y8888P"        888  888    Y88b
 *
 *    - 64-bit 680x0-inspired Virtual Machine and assembler -
 */

#include <cstdlib>
#include <new>
#include "mc64k.hpp"
#include "loader/binary.hpp"

using namespace MC64K::Loader;

/**
 * Binary Constructor
 */
Binary::Binary(const char* sFileName) :
    sFileName(sFileName),
    pFileHandle(0), pManifest(0),
    uManifestLength(0)
{
    pFileHandle = std::fopen(sFileName, "rb");
    if (!pFileHandle) {
        throw Error(sFileName, "file could not be opened for input");
    }
    uint64 aHeader[2] = { 0, 0 };
    readChunkHeader(aHeader, FILE_MAGIC_ID);
}

/**
 * Binary Destructor
 */
Binary::~Binary() {
    if (pFileHandle) {
        std::fclose(pFileHandle);
        pFileHandle = 0;
    }
    if (pManifest) {
        std::free(pManifest);
    }
}

/**
 * Attempts to read the expected chunk header
 */
void Binary::readChunkHeader(uint64 *pHeader, uint64 const uExpectedID) {
    if (2 != std::fread(pHeader, sizeof(uint64), 2, pFileHandle)) {
        throw Error(sFileName, "failed to load header");
    }
    if (uExpectedID != pHeader[0]) {
        throw Error(sFileName, "invalid header ID", pHeader[0]);
    }
}

/**
 * Attempts to load and allocate the Chunk List
 */
void Binary::loadManifest() {
    uint64 aHeader[2] = { 0, 0 };
    readChunkHeader(aHeader, CHUNK_MANIFEST_ID);

    pManifest = (ManifestEntry*)std::malloc(aHeader[1]);

    if (!pManifest) {
        throw Error(sFileName, "unable to allocate chunk", CHUNK_MANIFEST_ID);
    }

    uManifestLength = aHeader[1] / sizeof(ManifestEntry);

    if (uManifestLength != std::fread(pManifest, sizeof(ManifestEntry), uManifestLength, pFileHandle)) {
        throw Error(sFileName, "failed to load chunk", CHUNK_MANIFEST_ID);
    }
    for (uint32 u = 0; u < uManifestLength; ++u) {
        std::printf(
            "\tChunk %.*s found at offset %ld\n",
            8,
            (const char*)(&pManifest[u].uMagicID),
            pManifest[u].iOffset
        );
    }
}

/**
 * Locate a chunk by ID
 */
const Binary::ManifestEntry* Binary::findChunk(const uint64 uChunkID) {
    for (uint32 u = 0; u < uManifestLength; ++u) {
        if (uChunkID == pManifest[u].uMagicID) {
            return &pManifest[u];
        }
    }
    throw Error(sFileName, "missing chunk", uChunkID);
}

/**
 * Allocate and load a chunk by ID
 */
uint8* Binary::readChunkData(const uint64 uChunkID) {
    uint64 aHeader[2] = { 0, 0 };
    uint64 uAllocSize = 0;
    uint8* pRawData   = 0;
    const ManifestEntry* pManifestEntry = findChunk(uChunkID);

    std::fseek(pFileHandle, pManifestEntry->iOffset, SEEK_SET);
    readChunkHeader(aHeader, uChunkID);
    uAllocSize = alignSize(aHeader[1]);
    pRawData   = (uint8*)std::malloc(uAllocSize);
    if (!pRawData) {
        return 0;
    }
    if (uAllocSize != std::fread(pRawData, 1, uAllocSize, pFileHandle)) {
        std::free(pRawData);
        return 0;
    }
    return pRawData;
}

/**
 * Attempts to load and return the Executable
 */
const Executable* Binary::load() {
    loadManifest();
    uint8*      pByteCode   = 0;
    uint8*      pExportList = 0;
    uint8*      pImportList = 0;
    Executable* pExecutable = 0;

    if (
        (pByteCode   = readChunkData(CHUNK_BYTE_CODE_ID)) &&
        (pExportList = readChunkData(CHUNK_EXPORT_LIST_ID)) &&
        (pImportList = readChunkData(CHUNK_IMPORT_LIST_ID)) &&
        (pExecutable = new (std::nothrow) Executable(pExportList, pImportList, pByteCode))
    ) {
        return pExecutable;
    }

    std::free(pByteCode);
    std::free(pImportList);
    std::free(pExportList);
    throw Error(sFileName, "unable to load binary");
}

/**
 * Executable Constructor
 *
 * Private, sanity checks performed before getting here.
 */
Executable::Executable(const uint8* pRawExportData, const uint8* pRawImportData, const uint8* pRawByteCode) :
    pExportData(pRawExportData),
    pImportData(pRawImportData),
    pByteCode(pRawByteCode),
    pExportedSymbols(0),
    pImportedSymbols(0),
    uNumExportedSymbols(0),
    uNumImportedSymbols(0)
{
    if (
        (uNumExportedSymbols = *(uint32*)pExportData) &&
        (pExportedSymbols    = (LinkSymbol*)std::malloc(uNumExportedSymbols * sizeof(LinkSymbol)))
    ) {
        const uint32* pOffsets = (uint32*)(pExportData + 4);
        char* pName = ((char*)pExportData) + 4 + uNumExportedSymbols * sizeof(uint32);
        for (unsigned u = 0; u < uNumExportedSymbols; ++u) {
            pExportedSymbols[u].sIdentifier = pName;
            pExportedSymbols[u].pByteCode   = pRawByteCode + pOffsets[u];

            uint8 uByte;

            // Advance to the next name
            // Advance to the next name
            while ((uByte = *pName) > 7) {
                ++pName;
            }
            pExportedSymbols[u].uFlags = uByte;

            // Null terminate
            *pName++ = 0;
        }
    }

    if (
        (uNumImportedSymbols = *(uint32*)pImportData) &&
        (pImportedSymbols    = (LinkSymbol*)std::malloc(uNumImportedSymbols * sizeof(LinkSymbol)))
    ) {
        char*  pName   = ((char*)pImportData) + 4;
        for (unsigned u = 0; u < uNumImportedSymbols; ++u) {
            pImportedSymbols[u].sIdentifier = pName;
            pImportedSymbols[u].pRawData    = 0;

            uint8 uByte;

            // Advance to the next name
            // Advance to the next name
            while ((uByte = *pName) > 7) {
                ++pName;
            }
            pImportedSymbols[u].uFlags = uByte;

            // Null terminate
            *pName++ = 0;
        }
    }
}

/**
 * Executable destructor
 */
Executable::~Executable() {
    std::free((void*)pExportedSymbols);
    std::free((void*)pImportedSymbols);
    std::free((void*)pImportData);
    std::free((void*)pExportData);
    std::free((void*)pByteCode);
}

