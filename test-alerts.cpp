
#include "typewise-alert.h"

#include "gtest/gtest.h"


TEST(TypewiseAlert, InferBreachAsPerLimits) {
    ASSERT_TRUE(inferBreach(19.0, 20.0, 30.0) == BreachType::TOO_LOW);
    ASSERT_TRUE(inferBreach(0.0, 20.0, 30.0) == BreachType::TOO_LOW);
    ASSERT_TRUE(inferBreach(20.0, 20.0, 30.0) == BreachType::NORMAL);
    ASSERT_TRUE(inferBreach(21.0, 20.0, 30.0) == BreachType::NORMAL);
    ASSERT_TRUE(inferBreach(29.0, 20.0, 30.0) == BreachType::NORMAL);
    ASSERT_TRUE(inferBreach(30.0, 20.0, 30.0) == BreachType::NORMAL);
    ASSERT_TRUE(inferBreach(31.0, 20.0, 30.0) == BreachType::TOO_HIGH);
    ASSERT_TRUE(inferBreach(50.0, 20.0, 30.0) == BreachType::TOO_HIGH);
}

TEST(TypewiseAlert, ClassifyTemperatureBreach) {
    
    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::PASSIVE_COOLING, -1) == BreachType::TOO_LOW);
    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::PASSIVE_COOLING, 0) == BreachType::NORMAL);
    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::PASSIVE_COOLING, 1) == BreachType::NORMAL);
    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::PASSIVE_COOLING, 34) == BreachType::NORMAL);
    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::PASSIVE_COOLING, 35) == BreachType::NORMAL);
    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::PASSIVE_COOLING, 36) == BreachType::TOO_HIGH);
    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::PASSIVE_COOLING, 40) == BreachType::TOO_HIGH);
    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::PASSIVE_COOLING, 45) == BreachType::TOO_HIGH);

    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::MED_ACTIVE_COOLING, -1) == BreachType::TOO_LOW);
    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::MED_ACTIVE_COOLING, 0) == BreachType::NORMAL);
    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::MED_ACTIVE_COOLING, 35) == BreachType::NORMAL);
    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::MED_ACTIVE_COOLING, 39) == BreachType::NORMAL);
    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::MED_ACTIVE_COOLING, 40) == BreachType::NORMAL);
    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::MED_ACTIVE_COOLING, 41) == BreachType::TOO_HIGH);
    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::MED_ACTIVE_COOLING, 45) == BreachType::TOO_HIGH);

    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::HI_ACTIVE_COOLING, -1) == BreachType::TOO_LOW);
    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::HI_ACTIVE_COOLING, 0) == BreachType::NORMAL);
    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::HI_ACTIVE_COOLING, 35) == BreachType::NORMAL);
    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::HI_ACTIVE_COOLING, 44) == BreachType::NORMAL);
    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::HI_ACTIVE_COOLING, 45) == BreachType::NORMAL);
    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::HI_ACTIVE_COOLING, 46) == BreachType::TOO_HIGH);
    ASSERT_TRUE(classifyTemperatureBreach(CoolingType::HI_ACTIVE_COOLING, 50) == BreachType::TOO_HIGH);
}

TEST(TypewiseAlert, SendToController)
{
    double temperatureInC = 20.0;

    BreachType breachType = classifyTemperatureBreach(CoolingType::HI_ACTIVE_COOLING, temperatureInC);
    ASSERT_TRUE(OK == sendToController(breachType));

}

TEST(TypewiseAlert, sendToEmail)
{

    BreachType breachType1 = classifyTemperatureBreach(CoolingType::PASSIVE_COOLING, -1);
    BreachType breachType2 = classifyTemperatureBreach(CoolingType::HI_ACTIVE_COOLING, 46);
    BreachType breachType3 = classifyTemperatureBreach(CoolingType::MED_ACTIVE_COOLING, 35);

    ASSERT_TRUE(OK == sendToEmail(breachType1));
    ASSERT_TRUE(OK == sendToEmail(breachType2));
    ASSERT_TRUE(OK == sendToEmail(breachType3));

}

TEST(TypewiseAlert, CheckAndAlertTest)
{
    AlertTarget alertTarget = {};
    EquipmentCharacter characteristic = {};
    double temperatureInC = 0;

    alertTarget = AlertTarget::TO_EMAIL;
    characteristic.coolingType = CoolingType::PASSIVE_COOLING;
    temperatureInC = 10;
    ASSERT_TRUE(OK == checkAndAlert(alertTarget, characteristic, temperatureInC));

    alertTarget = AlertTarget::TO_CONTROLLER;
    characteristic.coolingType = CoolingType::PASSIVE_COOLING;
    temperatureInC = 10;
    ASSERT_TRUE(OK == checkAndAlert(alertTarget, characteristic, temperatureInC));

}

