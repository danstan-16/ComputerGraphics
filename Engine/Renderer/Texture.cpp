#include "pch.h"
#include "Texture.h"
#include "Renderer.h"
#include <SDL3_image/SDL_image.h>

namespace nu
{
	Texture::~Texture()
	{
		if (m_gpuDevice)
		{
			if (m_gpuSampler) SDL_ReleaseGPUSampler(m_gpuDevice, m_gpuSampler);
			if (m_gpuTexture) SDL_ReleaseGPUTexture(m_gpuDevice, m_gpuTexture);
		}
	}

	bool Texture::Load(const std::string& filename, const Renderer& renderer)
	{
		m_gpuDevice = renderer.m_gpuDevice;

		SDL_Surface* loadedSurface = IMG_Load(filename.c_str()); 
		if (loadedSurface == nullptr)
		{
			std::cerr << "Could not load image " << filename << ": " << SDL_GetError() << std::endl;

			return false;
		}

		SDL_Surface* surface = SDL_ConvertSurface(loadedSurface, SDL_PIXELFORMAT_ABGR8888);
		SDL_DestroySurface(loadedSurface);
		if (surface == nullptr)
		{
			std::cerr << "Could not convert image " << filename << ": " << SDL_GetError() << std::endl;

			return false;
		}

		m_width = static_cast<uint32_t>(surface->w);
		m_height = static_cast<uint32_t>(surface->h);

		uint32_t imageSize = static_cast<uint32_t>(surface->pitch) * m_height;

		SDL_GPUTextureCreateInfo textureCreateInfo{
			.type = SDL_GPU_TEXTURETYPE_2D, 
			.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM, 
			.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER, 
			.width = m_width,
			.height = m_height,
			.layer_count_or_depth = 1,
			.num_levels = 1
		};

		m_gpuTexture = SDL_CreateGPUTexture(m_gpuDevice, &textureCreateInfo); // todo
		if (m_gpuTexture == nullptr)
		{
			std::cerr << "Could not create texture: " << SDL_GetError() << std::endl;

			SDL_DestroySurface(surface);

			return false;
		}

		SDL_GPUTransferBufferCreateInfo transferBufferCreateInfo{
			.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD, 
			.size = imageSize 
		};

		SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer(m_gpuDevice, &transferBufferCreateInfo); 
		if (transferBuffer == nullptr)
		{
			std::cerr << "Could not create transfer buffer: " << SDL_GetError() << std::endl;

			SDL_DestroySurface(surface);
			SDL_ReleaseGPUTexture(m_gpuDevice, m_gpuTexture);
			m_gpuTexture = nullptr;

			return false;
		}

		uint8_t* transferData = static_cast<uint8_t*>(SDL_MapGPUTransferBuffer(m_gpuDevice, transferBuffer, false)); 
		if (transferData == nullptr)
		{
			std::cerr << "Could not map transfer buffer: " << SDL_GetError() << std::endl;

			SDL_DestroySurface(surface);
			SDL_ReleaseGPUTransferBuffer(m_gpuDevice, transferBuffer);
			SDL_ReleaseGPUTexture(m_gpuDevice, m_gpuTexture);
			m_gpuTexture = nullptr;

			return false;
		}

		memcpy(transferData, surface->pixels, imageSize);

		SDL_UnmapGPUTransferBuffer(m_gpuDevice, transferBuffer);

		uint32_t pixelsPerRow = static_cast<uint32_t>(surface->pitch) / 4;
		SDL_DestroySurface(surface);

		SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(m_gpuDevice); 
		if (commandBuffer == nullptr)
		{
			std::cerr << "Could not acquire GPU command buffer: " << SDL_GetError() << std::endl;

			SDL_ReleaseGPUTransferBuffer(m_gpuDevice, transferBuffer);
			SDL_ReleaseGPUTexture(m_gpuDevice, m_gpuTexture);
			m_gpuTexture = nullptr;

			return false;
		}

		SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(commandBuffer); 

		SDL_GPUTextureTransferInfo transferInfo{
			.transfer_buffer = transferBuffer, 
			.offset = 0,
			.pixels_per_row = pixelsPerRow,
			.rows_per_layer = m_height
		};

		SDL_GPUTextureRegion textureRegion{
			.texture = m_gpuTexture, 
			.mip_level = 0,
			.layer = 0,
			.x = 0,
			.y = 0,
			.z = 0,
			.w = m_width, 
			.h = m_height, 
			.d = 1
		};

		SDL_UploadToGPUTexture(copyPass, &transferInfo, &textureRegion, false);

		SDL_EndGPUCopyPass(copyPass);

		if (!SDL_SubmitGPUCommandBuffer(commandBuffer))
		{
			std::cerr << "Could not submit GPU command buffer: " << SDL_GetError() << std::endl;

			SDL_ReleaseGPUTransferBuffer(m_gpuDevice, transferBuffer);
			SDL_ReleaseGPUTexture(m_gpuDevice, m_gpuTexture);
			m_gpuTexture = nullptr;

			return false;
		}

		SDL_ReleaseGPUTransferBuffer(m_gpuDevice, transferBuffer);

		SDL_GPUSamplerCreateInfo samplerCreateInfo{
			.min_filter = SDL_GPU_FILTER_LINEAR, 
			.mag_filter = SDL_GPU_FILTER_NEAREST, 
			.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR,
			.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE, 
			.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_MIRRORED_REPEAT, 
			.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT
		};

		m_gpuSampler = SDL_CreateGPUSampler(m_gpuDevice, &samplerCreateInfo); 
		if (m_gpuSampler == nullptr)
		{
			std::cerr << "Could not create sampler: " << SDL_GetError() << std::endl;

			SDL_ReleaseGPUTexture(m_gpuDevice, m_gpuTexture);
			m_gpuTexture = nullptr;

			return false;
		}

		return true;
	}
}
