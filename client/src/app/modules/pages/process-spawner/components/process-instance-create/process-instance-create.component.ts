import { AfterViewInit, Component } from '@angular/core';
import { Form, FormArray, FormBuilder, FormControl, FormGroup, Validators } from '@angular/forms';
import { DialogComponentRef } from '../../../../shared/dialog/dialog-component-ref';
import { ProcessType } from '../../enums/process-type.enum';
import { tap } from 'rxjs';
import { ProcessSpawnRequestDto } from '../../models/process-spawn-request-dto';

@Component({
  selector: 'app-process-instance-create',
  templateUrl: './process-instance-create.component.html',
  styleUrl: './process-instance-create.component.scss'
})
export class ProcessInstanceCreateComponent implements AfterViewInit{
    data: any;
    outputData: ProcessSpawnRequestDto;
    dynamicForm: FormGroup;

    constructor(private formBuilder: FormBuilder) {
        this.dynamicForm = this.formBuilder.group({
            process_type: ['', Validators.required],
            parameters: this.formBuilder.array([]),
        });
    }

    ngAfterViewInit() {
        this.dynamicForm.valueChanges.pipe(tap(value => {
            const processSpawnRequestDto: ProcessSpawnRequestDto = {
                process_type: value.process_type,
                parameters: new Map(value.parameters.map((param: { key: string; value: string; }) => [param.key, param.value])),
            };

            this.outputData = processSpawnRequestDto;
        })).subscribe();
    }

    processTypeKeys = Object.keys(ProcessType).filter(key => isNaN(Number(key)));
    processTypes = this.getProcessTypePairs();

    getProcessTypePairs() {
        return this.processTypeKeys.map(key => ({
            key,
            value: ProcessType[key as keyof typeof ProcessType]
        }));
    }

    getParams(): FormArray {
        return this.dynamicForm.get('parameters') as FormArray;
    }

    getType(): FormControl {
        return this.dynamicForm.get('type') as FormControl;
    }

    addParam(): void {
      const optionGroup = this.formBuilder.group({
        key: ['', Validators.required],
        value: ['', Validators.required],
      });

      this.getParams().push(optionGroup);
    }

    removeParam(index: number): void {
        this.getParams().removeAt(index);
    }

    validate(): boolean {
        if (this.dynamicForm.valid) {
            return true;
        }

        const errors = this.getErrors();
        const serializableErrors = errors.map((errMap) => {
            return Object.fromEntries(errMap);
        });
        
        throw JSON.stringify(serializableErrors);
    }

    getErrors(): Array<Map<string, string[]>> {
        let errors = new Array<Map<string, string[]>>();
        if (this.getType().getError('required')) {
            errors.push(new Map<string, string[]>([['Type', ['required']]]));
        }

        this.getParams().controls.forEach((control, index) => {
            let keyErrors = new Map<string, string[]>();
            if (control.get('key')?.getError('required')) {
                keyErrors.set('Param ' + index + ' key ', ['required']);
            }

            let valueErrors = new Map<string, string[]>();
            if (control.get('value')?.getError('required')) {
                valueErrors.set('Param ' + index + ' value ', ['required']);
            }

            if (keyErrors.size > 0) {
                errors.push(keyErrors);
            }

            if (valueErrors.size > 0) {
                errors.push(valueErrors);
            }
        });

        return errors;
    }
}
