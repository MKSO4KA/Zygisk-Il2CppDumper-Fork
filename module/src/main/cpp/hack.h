//
// Created by Perfare on 2020/7/4.
//

#ifndef ZYGISK_IL2CPPDUMPER_HACK_H
#define ZYGISK_IL2CPPDUMPER_HACK_H

#include <stddef.h>
#include <jni.h>

// Глобальный указатель на JavaVM для безопасного использования в разных потоках
extern JavaVM *g_vm;

void hack_prepare(const char *game_data_dir, void *data, size_t length);

#endif //ZYGISK_IL2CPPDUMPER_HACK_H