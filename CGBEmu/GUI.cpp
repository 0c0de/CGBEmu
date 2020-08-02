#include "GUI.h"

bool isActiveMemoryViewer = true;

bool isActiveFlags = true;
bool initiatedLoopMMU = false;
bool showROM = false;
bool showHRAM = false;
bool showRAM = false;
bool showVRAM = false;
bool showWRAM = false;
bool showIO = false;
bool showSPRITEATTR = false;
bool showGSA = false;

static MemoryEditor ROMData;

void drawMMU(MMU *mmu) {
	if (isActiveMemoryViewer) {

		ImGui::Begin("MMU Viewer", &isActiveMemoryViewer, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("View ROM", "Ctrl+O", &showROM);
				ImGui::MenuItem("View VRAM", "Ctrl+O", &showVRAM);
				ImGui::MenuItem("View RAM", "Ctrl+O", &showRAM);
				ImGui::MenuItem("View HRAM", "Ctrl+O", &showHRAM);
				ImGui::MenuItem("View WRAM", "Ctrl+O", &showWRAM);
				ImGui::MenuItem("View IO", "Ctrl+O", &showIO);
				ImGui::MenuItem("View SPRITEATTR", "Ctrl+O", &showSPRITEATTR);
				ImGui::MenuItem("View Graphic Special Adressses", "Ctrl+O", &showGSA);

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (showROM) {
			ImGui::Text("ROM");
			ROMData.DrawContents((void*)(mmu->rom), 0x8000);
		}

		if (showHRAM) {
			ImGui::Text("HRAM");
			ROMData.DrawContents((void*)(mmu->internal_ram), 0x7F);
		}

		if (showRAM) {
			ImGui::Text("RAM");
			ROMData.DrawContents((void*)(mmu->ram), 0xBFFF);
		}

		if (showVRAM) {
			ImGui::Text("VRAM");
			ROMData.DrawContents((void*)(mmu->vram), 0x9FFF);
		}

		if (showWRAM) {
			ImGui::Text("WRAM");
			ROMData.DrawContents((void*)(mmu->wram), 0xDFFF);
		}

		if (showIO) {
			ImGui::Text("IO");
			ROMData.DrawContents((void*)(mmu->io), 0x4C);
		}

		if (showSPRITEATTR) {
			ImGui::Text("SPRITEATTR");
			ROMData.DrawContents((void*)(mmu->sprite_attrib), 0xA0);
		}
		if (showGSA) {
			ImGui::Text("Special video addresses");
			ImGui::Spacing();

			ImGui::Text("0xFF40: %02x -> LCD/GPU Control", mmu->read8(0xFF40));
			ImGui::Text("0xFF42: %02x -> Scroll Y", mmu->read8(0xFF42));
			ImGui::Text("0xFF43: %02x -> Scroll X", mmu->read8(0xFF43));
			ImGui::Text("0xFF44: %02x -> Current Scan Line", mmu->read8(0xFF44));
			ImGui::Text("0xFF47: %02x -> Backgorund palette", mmu->read8(0xFF47));

			ImGui::Text("Stack Pointer: %04x", mmu->sp);
		}

		ImGui::End();
	}
}

void drawFlags(GameboyFlags *flags, GameboyRegisters *reg, GPU *gpu) {
	ImGui::Begin("Flag State Viewer", &isActiveFlags, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	if (flags->Z) {
		ImGui::Text("Zero Flag: 1");
	}
	else {
		ImGui::Text("Zero Flag: 0");
	}

	if (flags->C) {
		ImGui::Text("Carry Flag: 1");
	}
	else {
		ImGui::Text("Carry Flag: 0");
	}

	if (flags->H) {
		ImGui::Text("Half-Carry Flag: 1");
	}
	else {
		ImGui::Text("Half-Carry Flag: 0");
	}

	if (flags->N) {
		ImGui::Text("Substract Flag: 1");
	}
	else {
		ImGui::Text("Substract Flag: 0");
	}

	ImGui::Spacing();

	ImGui::Text("Registers");
	ImGui::BeginGroup();
	ImGui::Text("Register A: %02x |", reg->A);
	ImGui::SameLine();
	ImGui::Text("Register F: %02x |", reg->F);
	ImGui::SameLine();
	ImGui::Text("Register AF: %04x", reg->AF);
	ImGui::Text("Register B: %02x |", reg->B);
	ImGui::SameLine();
	ImGui::Text("Register C: %02x |", reg->C);
	ImGui::SameLine();
	ImGui::Text("Register BC: %04x", reg->BC);
	ImGui::Text("Register D: %02x |", reg->D);
	ImGui::SameLine();
	ImGui::Text("Register E: %02x |", reg->E);
	ImGui::SameLine();
	ImGui::Text("Register DE: %04x", reg->DE);
	ImGui::Text("Register H: %02x |", reg->H);
	ImGui::SameLine();
	ImGui::Text("Register L: %02x |", reg->L);
	ImGui::SameLine();
	ImGui::Text("Register HL: %04x", reg->HL);
	ImGui::EndGroup();

	ImGui::Spacing();

	ImGui::BeginGroup();
	ImGui::Text("GPU Clock: %d", gpu->clock);
	ImGui::Text("GPU Mode: %d", gpu->mode);
	ImGui::EndGroup();
	
	ImGui::End();
}