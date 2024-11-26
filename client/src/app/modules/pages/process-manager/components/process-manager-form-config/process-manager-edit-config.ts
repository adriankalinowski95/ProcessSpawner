import { ActionHandlerType, DynamicFormConfig, DynamicFormControlType, ErrorHandlerType, GuiValidatorType } from "app/modules/shared/dynamic-form/models/dynamic-form-config";
import { CustomInputComponent } from "app/modules/shared/form-components/custom-input/custom-input.component";
import { ProcessManagerDto } from "../../models/process-manager-dto";

export function getEditConfig(processManager: ProcessManagerDto) : DynamicFormConfig {
    return  {
        stepper: false,
        title: 'Edit process manager',
        errorHandlerType: ErrorHandlerType.Outer,
        actionPanelConfig: {
          actionHandlerType: ActionHandlerType.Inner,
        },
        data: [
        {
            groupName: 'nameGroup',
            formControlsData: [
                {
                    key: 'name',
                    value: processManager.name,
                    label: 'Name',
                    guiValidators: [
                        { type: GuiValidatorType.MaxLength, value: 25 }, 
                        { type: GuiValidatorType.Required, value: true }
                    ],
                    customValidators: [],
                    type: DynamicFormControlType.Input,
                    component: CustomInputComponent,
                },
            ]
        },
        {
          groupName: 'networkGroup',
          formControlsData: [
            {
                key: 'address',
                value: processManager.address,
                label: 'Address',
                guiValidators: [
                    { type: GuiValidatorType.MaxLength, value: 25 }, 
                    { type: GuiValidatorType.Required, value: true }
                ],
                customValidators: [],
                type: DynamicFormControlType.Input,
                component: CustomInputComponent,
            },
            {
                key: 'port',
                value: processManager.port,
                label: 'Port',
                guiValidators: [
                    { type: GuiValidatorType.MaxLength, value: 10 }, 
                    { type: GuiValidatorType.Required, value: true }
                ],
                customValidators: [],
                type: DynamicFormControlType.Input,
                component: CustomInputComponent,
            }
          ]
        }
        ]
    };
}