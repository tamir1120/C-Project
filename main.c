#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "queue.h"
#include "supporting_functions.h"
#include "semphr.h"
/* Used as a loop counter to create a very crude delay. */
#define mainDELAY_LOOP_COUNT		( 0xffffff )

/*Mobility Police*/
const char* pcPoliceForTask1 = "Police car number 1 is running\r\n";
const char* pcPoliceForTask2 = "Police car number 2 is running\r\n";
const char* pcPoliceForTask3 = "Police car number 3 is running\r\n";
/*--------------------------------------------------------------------------------------------------------------------*/
/*Mobility Fire*/
const char* pcFireForTask1 = "Fire car number 1 is running\r\n";
const char* pcFireForTask2 = "Fire car number 2 is running\r\n";
const char* pcFireForTask3 = "Fire car number 3 is running\r\n";
/*--------------------------------------------------------------------------------------------------------------------*/
/*Mobility Ambulance*/
const char* pcAmbulanceForTask1 = "Ambulance car number 1 is running\r\n";
const char* pcAmbulanceForTask2 = "Ambulance car number 2 is running\r\n";
const char* pcAmbulanceForTask3 = "Ambulance car number 3 is running\r\n";
const char* pcAmbulanceForTask4 = "Ambulance car number 4 is running\r\n";
/*--------------------------------------------------------------------------------------------------------------------*/
/*Mobility Corona*/
const char* pcCoronaForTask1 = "Corona car number 1 is running\r\n";
const char* pcCoronaForTask2 = "Corona car number 2 is running\r\n";
const char* pcCoronaForTask3 = "Corona car number 3 is running\r\n";
const char* pcCoronaForTask4 = "Corona car number 4 is running\r\n";
/*--------------------------------------------------------------------------------------------------------------------*/

/*Queue Message*/
static void vTaskDispacher(void* pvParameters); //Dispacher
static void vTaskPolice(void* pvParameters); // Police
static void vTaskPoliceTeam(void* pvParameters); // Police Mobility
static void vTaskFire(void* pvParameters); // Fire
static void vTaskFireTeam(void* pvParameters); // Fire Mobility
static void vTaskAmbulance(void* pvParameters); // Ambulance
static void vTaskAmbulanceteam(void* pvParameters); // Ambulance Mobility
static void vTaskCorona(void* pvParameters); // Corona
static void vTaskCoronaTeam(void* pvParameters); // Corona Mobility


QueueHandle_t xQueuePolice;
QueueHandle_t xQueueFire; 
QueueHandle_t xQueueAmbulance; 
QueueHandle_t xQueueCorona;

/*--------------------------------------------------------------------------------------------------------------------*/


/* Declare a variable of type SemaphoreHandle_t.  This is used to reference the
semaphore that is used to synchronize a task with an interrupt. */
/*--------------------------------------------------------------------------------------------------------------------*/
/*Semaphore*/
SemaphoreHandle_t xCountingSemaphorePolice; //Police
SemaphoreHandle_t xCountingSemaphoreFire; //Fire
SemaphoreHandle_t xCountingSemaphoreAmbulance; //Ambulance
SemaphoreHandle_t xCountingSemaphoreCorona; //Corona

/*--------------------------------------------------------------------------------------------------------------------*/
enum suit {
	Police = 0,
	Fire = 1,
	Ambulance = 2,
	Corona = 3,
} op;


int main( void )
{
	/*--------------------------------------------------------------------------------------------------------------------*/
	/*Semaphore*/
	xCountingSemaphorePolice = xSemaphoreCreateCounting(10, 0); 
	xCountingSemaphoreFire = xSemaphoreCreateCounting(10, 0); 
	xCountingSemaphoreAmbulance = xSemaphoreCreateCounting(10, 0); 
	xCountingSemaphoreCorona = xSemaphoreCreateCounting(10, 0); 
	
	/*--------------------------------------------------------------------------------------------------------------------*/
    /*Queue Message*/
	xQueuePolice = xQueueCreate(10, sizeof(int)); //police
	xQueueFire = xQueueCreate(10, sizeof(int)); // FIRE
	xQueueAmbulance = xQueueCreate(10, sizeof(int)); //Ambulance
	xQueueCorona = xQueueCreate(10, sizeof(int)); //corona

	if (xQueuePolice != NULL || xQueueFire != NULL || xQueueAmbulance != NULL || xQueueCorona != NULL || xCountingSemaphorePolice != NULL || xCountingSemaphoreFire != NULL || xCountingSemaphoreAmbulance != NULL || xCountingSemaphoreCorona != NULL) 
	{
		xTaskCreate(vTaskDispacher, "Dispatcher", 1000, (void*)100, 1, NULL);
		xTaskCreate(vTaskPolice, "Police", 1000, NULL, 1, NULL);
		xTaskCreate(vTaskFire, "Fire", 1000, NULL, 1, NULL);
		xTaskCreate(vTaskAmbulance, "Ambulance", 1000, NULL, 1, NULL);
		xTaskCreate(vTaskCorona, "Corona", 1000, NULL, 1, NULL);

		xTaskCreate(vTaskPoliceTeam, "Police car number 1", 1000, (void*)pcPoliceForTask1, 1, NULL);
		xTaskCreate(vTaskPoliceTeam, "Police car number 2", 1000, (void*)pcPoliceForTask2, 1, NULL);
		xTaskCreate(vTaskPoliceTeam, "Police car number 3", 1000, (void*)pcPoliceForTask3, 1, NULL);
	
		xTaskCreate(vTaskFireTeam, "Fire car number 1", 1000, (void*)pcFireForTask1, 1, NULL);
		xTaskCreate(vTaskFireTeam, "Fire car number 2", 1000, (void*)pcFireForTask2, 1, NULL);
		xTaskCreate(vTaskFireTeam, "Fire car number 3", 1000, (void*)pcFireForTask3, 1, NULL);

		xTaskCreate(vTaskAmbulanceteam, "Ambulance car number 1", 1000, (void*)pcAmbulanceForTask1, 1, NULL);
		xTaskCreate(vTaskAmbulanceteam, "Ambulance car number 2", 1000, (void*)pcAmbulanceForTask2, 1, NULL);
		xTaskCreate(vTaskAmbulanceteam, "Ambulance car number 3", 1000, (void*)pcAmbulanceForTask3, 1, NULL);
		xTaskCreate(vTaskAmbulanceteam, "Ambulance car number 4", 1000, (void*)pcAmbulanceForTask4, 1, NULL);

		xTaskCreate(vTaskCoronaTeam, "Corona car number 1", 1000, (void*)pcCoronaForTask1, 1, NULL);
		xTaskCreate(vTaskCoronaTeam, "Corona car number 2", 1000, (void*)pcCoronaForTask2, 1, NULL);
		xTaskCreate(vTaskCoronaTeam, "Corona car number 3", 1000, (void*)pcCoronaForTask3, 1, NULL);
		xTaskCreate(vTaskCoronaTeam, "Corona car number 4", 1000, (void*)pcCoronaForTask4, 1, NULL);
		
		/* Start the scheduler to start the tasks executing. */
		vTaskStartScheduler();
	}
	else
	{
		printf("The queue could not be created.\n\r");
	}

	/*--------------------------------------------------------------------------------------------------------------------*/

	for (;; );
	return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------Dispacher----------------*/
static void vTaskDispacher( void *pvParameters )
{
  int32_t lValueToSend;
  BaseType_t xStatus;
  lValueToSend = (int32_t)pvParameters;
 /*--------------------------------------------------------------------------------------------------------------------*/
/*Random Numbers*/
	
	srand((unsigned)time(NULL));
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
	const char *pcTaskName = "\r\nDispatcher is running\r\n";
	volatile uint32_t ul;

	for( ;; )
	{
		vPrintString( pcTaskName );
		op = rand() % 4;
		switch (op)
		{
			case Police:
				printf("The type of department is Police \r\n");
				lValueToSend = rand() % (4 + 1 - 0) + 1;
				printf("The numbers of cars is %d\r\n", lValueToSend);
				xStatus = xQueueSendToBack(xQueuePolice, &lValueToSend, 0); 
				if (xStatus != pdPASS)
				{
					vPrintString("Could not send to the queue.\r\n");
				}
				break;
			case Fire:
				printf("The type of department is Fire \r\n");
				lValueToSend = rand() % (4 + 1 - 0) + 1; //#define MAX_TASKS_CREATED_PER_SERVICE
				printf("The numbers of cars is %d\r\n", lValueToSend);
				xStatus = xQueueSendToBack(xQueueFire, &lValueToSend, 0); 
				if (xStatus != pdPASS)
				{
					vPrintString("Could not send to the queue.\r\n");
				}
				break;
			case Ambulance:
				printf("The type of department is Ambulance \r\n");
				lValueToSend = rand() % (4 + 1 - 0) + 1; //#define MAX_TASKS_CREATED_PER_SERVICE
				printf("The numbers of cars is %d\r\n", lValueToSend);
				xStatus = xQueueSendToBack(xQueueAmbulance, &lValueToSend, 0); 
				if (xStatus != pdPASS)
				{
					vPrintString("Could not send to the queue.\r\n");
				}
				break;
			case Corona:
				printf("The type of department is Corona \r\n");
				lValueToSend = rand() % (4 + 1 - 0) + 1; //#define MAX_TASKS_CREATED_PER_SERVICE
				printf("The numbers of cars is %d\r\n", lValueToSend);
				xStatus = xQueueSendToBack(xQueueCorona, &lValueToSend, 0);
				if (xStatus != pdPASS)
				{
					vPrintString("Could not send to the queue.\r\n");
				}
				break;
			default:
				printf("Error! operator is not correct \r\n");
		}
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

/*--------------------------------------------------------------------------------------------------------------------*/
//Police
/*--------------------------------------------------------------------------------------------------------------------*/
static void vTaskPolice( void *pvParameters )
{
		const char* pcTaskName = "Police is running\r\n";
		volatile uint32_t ul;
		int32_t lReceivedValue;
		BaseType_t xStatusPolice;
		const TickType_t xTicksToWait = pdMS_TO_TICKS(100UL);
		const TickType_t xDelay500ms = pdMS_TO_TICKS(1000UL);

		for (;; )
		{
			xStatusPolice = xQueueReceive(xQueuePolice, &lReceivedValue, portMAX_DELAY); 

			if (xStatusPolice == pdPASS) 
			{
				vPrintString(pcTaskName);
				vPrintStringAndNumber("Received = ", lReceivedValue); 
				while (lReceivedValue > 0)
				{
					xSemaphoreGive(xCountingSemaphorePolice); 
					lReceivedValue--;
				}
			}
			else
			{
				vPrintString("Could not receive from the queue.\r\n");
			}
	}
}

/*--------------------------------------------------------------------------------------------------------------------*/
//Fire
/*--------------------------------------------------------------------------------------------------------------------*/
static void vTaskFire(void* pvParameters)
{
		const char* pcTaskName = "Fire is running\r\n";
		volatile uint32_t ul;
		int32_t lReceivedValue;
		BaseType_t xStatusFire;
		const TickType_t xTicksToWait = pdMS_TO_TICKS(100UL);
		const TickType_t xDelay500ms = pdMS_TO_TICKS(1000UL);

		for (;; )
		{
			
			xStatusFire = xQueueReceive(xQueueFire, &lReceivedValue, portMAX_DELAY); 

			if (xStatusFire == pdPASS) //אם הצליח
			{
				vPrintString(pcTaskName);
				vPrintStringAndNumber("Received = ", lReceivedValue); 
				while (lReceivedValue > 0)
				{
					xSemaphoreGive(xCountingSemaphoreFire); 
					lReceivedValue--;
				}
			}
			else
			{
				vPrintString("Could not receive from the queue.\r\n");
			}
		}
}
/*--------------------------------------------------------------------------------------------------------------------*/
//Ambulance
/*--------------------------------------------------------------------------------------------------------------------*/
static void vTaskAmbulance(void* pvParameters)
{
		const char* pcTaskName = "Ambulance is running\r\n";
		volatile uint32_t ul;
		int32_t lReceivedValue;
		BaseType_t xStatusAmbulance;
		const TickType_t xTicksToWait = pdMS_TO_TICKS(100UL);
		const TickType_t xDelay500ms = pdMS_TO_TICKS(1000UL);

		for (;; )
		{
			
			xStatusAmbulance = xQueueReceive(xQueueAmbulance, &lReceivedValue, portMAX_DELAY); 

			if (xStatusAmbulance == pdPASS)
			{
				vPrintString(pcTaskName);
				vPrintStringAndNumber("Received = ", lReceivedValue); 
				while (lReceivedValue > 0)
				{
					xSemaphoreGive(xCountingSemaphoreAmbulance); 
					lReceivedValue--;
				}
			}
			else
			{
				vPrintString("Could not receive from the queue.\r\n");
			}
		}
}
/*--------------------------------------------------------------------------------------------------------------------*/
//Corona
/*--------------------------------------------------------------------------------------------------------------------*/
static void vTaskCorona(void* pvParameters)
{
		const char* pcTaskName = "Corona is running\r\n";
		volatile uint32_t ul;
		int32_t lReceivedValue;
		BaseType_t xStatusCorona;
		const TickType_t xTicksToWait = pdMS_TO_TICKS(100UL);
		const TickType_t xDelay500ms = pdMS_TO_TICKS(1000UL);

		for (;; )
		{
			
			xStatusCorona = xQueueReceive(xQueueCorona, &lReceivedValue, portMAX_DELAY); 

			if (xStatusCorona == pdPASS) 
			{
				vPrintString(pcTaskName);
				vPrintStringAndNumber("Received = ", lReceivedValue); 
				while (lReceivedValue > 0)
				{
					xSemaphoreGive(xCountingSemaphoreCorona); 
					lReceivedValue--;
				}
			}
			else
			{
				vPrintString("Could not receive from the queue.\r\n");
			}
		}
}


/*--------------------------------------------------------------------------------------------------------------------*/
		/*Semaphore Police*/
void vTaskPoliceTeam(void* pvParameters)
{
	char* pcTaskName;
	const TickType_t xDelay250ms = pdMS_TO_TICKS(250UL);//250ml
	pcTaskName = (char*)pvParameters;
		for (;; )
		{
			xSemaphoreTake(xCountingSemaphorePolice, portMAX_DELAY);
			vPrintString(pcTaskName);
			vTaskDelay(xDelay250ms);
		}
}
/*--------------------------------------------------------------------------------------------------------------------*/
		/*Semaphore Fire*/
void vTaskFireTeam(void* pvParameters)
{
	char* pcTaskName;
	const TickType_t xDelay250ms = pdMS_TO_TICKS(250UL);//250ml
	pcTaskName = (char*)pvParameters;
		for (;; )
		{
			xSemaphoreTake(xCountingSemaphoreFire, portMAX_DELAY);
			vPrintString(pcTaskName);
			vTaskDelay(xDelay250ms);
		}
}
/*--------------------------------------------------------------------------------------------------------------------*/
		/*Semaphore Ambulance*/
void vTaskAmbulanceteam(void* pvParameters)
{
	char* pcTaskName;
	const TickType_t xDelay250ms = pdMS_TO_TICKS(250UL);//250ml
	pcTaskName = (char*)pvParameters;
		for (;; )
		{
			xSemaphoreTake(xCountingSemaphoreAmbulance, portMAX_DELAY);
			vPrintString(pcTaskName);
			vTaskDelay(xDelay250ms);
		}
}
/*--------------------------------------------------------------------------------------------------------------------*/
		/*Semaphore Corona*/
void vTaskCoronaTeam(void* pvParameters)
{
	char* pcTaskName;
	const TickType_t xDelay250ms = pdMS_TO_TICKS(250UL);//250ml
	pcTaskName = (char*)pvParameters;
		for (;; )
		{
			xSemaphoreTake(xCountingSemaphoreCorona, portMAX_DELAY);
			vPrintString(pcTaskName);
			vTaskDelay(xDelay250ms);
		}
}