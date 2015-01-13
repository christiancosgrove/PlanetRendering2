//
//  RandomUtil.h
//  PlanetRendering
//
//  Created by Christian Cosgrove on 10/29/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//
#pragma once
#include <random>


class RandomUtils
{
public:
    static std::mt19937 rand;
    template<typename T>
    static T Normal(T mean, T variance)
    {
        std::normal_distribution<T> distribution(mean, variance);
        return distribution(rand);
    }
    template<typename T>
    static T Binomial(int mean, float variance) // This is the discrete analog of the normal distribution
    {
        std::binomial_distribution<T> distribution(mean, variance);
        return distribution(rand);
    }
    template<typename T>
    static T Uniform(T min, T max)
    {
        std::uniform_real_distribution<T> distribution(min, max);
        return distribution(rand);
    }
    
    static float UniformFloat()
    {
        return Uniform<float>(0.0f,1.0f);
    }
private:
};

