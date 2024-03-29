//#include "Audio3D.h"
//#ifdef _XBOX //Big-Endian
//#define fourccRIFF 'RIFF'
//#define fourccDATA 'data'
//#define fourccFMT 'fmt '
//#define fourccWAVE 'WAVE'
//#define fourccXWMA 'XWMA'
//#define fourccDPDS 'dpds'
//#endif
//
//#ifndef _XBOX //Little-Endian
//#define fourccRIFF 'FFIR'
//#define fourccDATA 'atad'
//#define fourccFMT ' tmf'
//#define fourccWAVE 'EVAW'
//#define fourccXWMA 'AMWX'
//#define fourccDPDS 'sdpd'
//#endif
//HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
//{
//    HRESULT hr = S_OK;
//    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
//        return HRESULT_FROM_WIN32(GetLastError());
//
//    DWORD dwChunkType{};
//    DWORD dwChunkDataSize{};
//    DWORD dwRIFFDataSize = 0;
//    DWORD dwFileType{};
//    DWORD bytesRead = 0;
//    DWORD dwOffset = 0;
//
//    while (hr == S_OK)
//    {
//        DWORD dwRead;
//        if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
//            hr = HRESULT_FROM_WIN32(GetLastError());
//
//        if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
//            hr = HRESULT_FROM_WIN32(GetLastError());
//
//        switch (dwChunkType)
//        {
//        case fourccRIFF:
//            dwRIFFDataSize = dwChunkDataSize;
//            dwChunkDataSize = 4;
//            if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
//                hr = HRESULT_FROM_WIN32(GetLastError());
//            break;
//
//        default:
//            if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
//                return HRESULT_FROM_WIN32(GetLastError());
//        }
//
//        dwOffset += sizeof(DWORD) * 2;
//
//        if (dwChunkType == fourcc)
//        {
//            dwChunkSize = dwChunkDataSize;
//            dwChunkDataPosition = dwOffset;
//            return S_OK;
//        }
//
//        dwOffset += dwChunkDataSize;
//
//        if (bytesRead >= dwRIFFDataSize) return S_FALSE;
//
//    }
//
//    return S_OK;
//
//}
//
//HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
//{
//    HRESULT hr = S_OK;
//    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
//        return HRESULT_FROM_WIN32(GetLastError());
//    DWORD dwRead;
//    if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
//        hr = HRESULT_FROM_WIN32(GetLastError());
//    return hr;
//}
//
//
//void Audio3D::Initialize() {
//	
//}
//
//void Audio3D::Update() {
//
//}
//
//HRESULT Audio3D::InitializeCOM() {
//	HRESULT hr;
//	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
//	if (FAILED(hr)) {
//		return hr;
//	}
//		
//
//	IXAudio2* pXAudio2 = nullptr;
//	if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR))) {
//		return hr;
//	}
//		
//
//	IXAudio2MasteringVoice* pMasterVoice = nullptr;
//	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice))) {
//		return hr;
//	}
//		
//	/* Microsoft::WRL::ComPtr<IXAudio2> XAudio2;
//	HRESULT hr;
//	if (FAILED(hr = XAudio2Create(&XAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
//		throw Platform::Exception::CreateException(hr);
//
//	IXAudio2MasteringVoice* pMasterVoice = nullptr;
//	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
//	return hr;*/
//
//}