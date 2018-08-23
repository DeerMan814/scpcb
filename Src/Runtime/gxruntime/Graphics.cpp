
#include "Graphics.h"
#include "Shader.h"
#include <stdlib.h>

using namespace GX;

Graphics::Graphics(int w, int h, bool fs) {
    window = new Window("GX", w, h, fs);

    defaultShader = new Shader(this, "default/");

    FT_Init_FreeType(&freeTypeLib);

    ID3D11Device* dxDevice = window->getDxDevice();

    dxVertexInputElemDesc[0].SemanticName = "POSITION";
    dxVertexInputElemDesc[0].SemanticIndex = 0;
    dxVertexInputElemDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    dxVertexInputElemDesc[0].InputSlot = 0;
    dxVertexInputElemDesc[0].AlignedByteOffset = 0;
    dxVertexInputElemDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    dxVertexInputElemDesc[0].InstanceDataStepRate = 0;

    dxVertexInputElemDesc[1].SemanticName = "NORMAL";
    dxVertexInputElemDesc[1].SemanticIndex = 0;
    dxVertexInputElemDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    dxVertexInputElemDesc[1].InputSlot = 0;
    dxVertexInputElemDesc[1].AlignedByteOffset = sizeof(float) * 3;
    dxVertexInputElemDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    dxVertexInputElemDesc[1].InstanceDataStepRate = 0;

    dxVertexInputElemDesc[2].SemanticName = "TEXCOORD";
    dxVertexInputElemDesc[2].SemanticIndex = 0;
    dxVertexInputElemDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
    dxVertexInputElemDesc[2].InputSlot = 0;
    dxVertexInputElemDesc[2].AlignedByteOffset = sizeof(float) * 6;
    dxVertexInputElemDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    dxVertexInputElemDesc[2].InstanceDataStepRate = 0;

    dxVertexInputElemDesc[3].SemanticName = "COLOR";
    dxVertexInputElemDesc[3].SemanticIndex = 0;
    dxVertexInputElemDesc[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    dxVertexInputElemDesc[3].InputSlot = 0;
    dxVertexInputElemDesc[3].AlignedByteOffset = sizeof(float) * 8;
    dxVertexInputElemDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    dxVertexInputElemDesc[3].InstanceDataStepRate = 0;

    dxDevice->CreateInputLayout(dxVertexInputElemDesc, 4, defaultShader->getDxVsCode(), defaultShader->getDxVsCodeLen() * sizeof(uint8_t), &dxVertexInputLayout);

    viewport = Rectanglei(0, 0, w, h);
    updateViewport();
    currentRenderTargetView = window->getBackBufferRtv();

    projectionMatrix = Matrix4x4f::identity;
    viewMatrix = Matrix4x4f::identity;
    Matrix4x4f tempWorldMat = Matrix4x4f::identity;

    FLOAT cbufferData[48];
    memcpy(cbufferData, projectionMatrix.elements, sizeof(FLOAT) * 16);
    memcpy(cbufferData + 16, viewMatrix.elements, sizeof(FLOAT) * 16);
    memcpy(cbufferData + 32, tempWorldMat.elements, sizeof(FLOAT) * 16);

    ZeroMemory(&dxMatrixCBufferDesc, sizeof(D3D11_BUFFER_DESC));
    dxMatrixCBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    dxMatrixCBufferDesc.ByteWidth = sizeof(FLOAT) * 48;
    dxMatrixCBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    dxMatrixCBufferDesc.CPUAccessFlags = 0;

    ZeroMemory(&dxMatrixCBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
    dxMatrixCBufferData.pSysMem = cbufferData;

    dxDevice->CreateBuffer(&dxMatrixCBufferDesc, &dxMatrixCBufferData, &dxMatrixCBuffer);

    ZeroMemory(&dxSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
    dxSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    dxSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    dxSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    dxSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    dxSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    dxSamplerDesc.MinLOD = 0;
    dxSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    dxDevice->CreateSamplerState(&dxSamplerDesc, &dxSamplerState);
}

Window* Graphics::getWindow() const {
    return window;
}

FT_Library* Graphics::getFreeTypeLibrary() {
    return &freeTypeLib;
}

void Graphics::swap(bool vsync) {
    window->swap(vsync);
}

Shader* Graphics::getDefaultShader() {
    return defaultShader;
}

void Graphics::updateDxCBuffer(Matrix4x4f worldMatrix) {
    ID3D11DeviceContext* dxContext = window->getDxContext();

    FLOAT cbufferData[48];
    memcpy(cbufferData, projectionMatrix.transpose().elements, sizeof(FLOAT) * 16);
    memcpy(cbufferData + 16, viewMatrix.transpose().elements, sizeof(FLOAT) * 16);
    memcpy(cbufferData + 32, worldMatrix.transpose().elements, sizeof(FLOAT) * 16);

    dxContext->UpdateSubresource(dxMatrixCBuffer, 0, NULL, cbufferData, 0, 0);
}

Graphics::~Graphics() {
    dxMatrixCBuffer->Release();
    dxSamplerState->Release();
    dxVertexInputLayout->Release();

    delete window;
}

void Graphics::update() {
    window->update();
    updateViewport();
}

void Graphics::updateViewport() {
    if (currentViewport.width() != viewport.width() ||
        currentViewport.height() != viewport.height() ||
        currentViewport.topLeftCorner().x != viewport.topLeftCorner().x ||
        currentViewport.topLeftCorner().y != viewport.topLeftCorner().y) {
        currentViewport = viewport;
        //TODO: how do viewport
    }
}

void Graphics::useVertexInputLayout() {
    ID3D11DeviceContext* dxContext = window->getDxContext();
    dxContext->IASetInputLayout(dxVertexInputLayout);
}

void Graphics::useMatrixCBuffer() {
    ID3D11DeviceContext* dxContext = window->getDxContext();
    dxContext->VSSetConstantBuffers(0, 1, &dxMatrixCBuffer);
}

void Graphics::useSampler() {
    ID3D11DeviceContext* dxContext = window->getDxContext();
    dxContext->PSSetSamplers(0, 1, &dxSamplerState);
}

void Graphics::clear(Color color) {
    updateViewport();

    float clearColor[4] = { color.red,color.green,color.blue,color.alpha };
    window->getDxContext()->ClearRenderTargetView(currentRenderTargetView, clearColor);
}

