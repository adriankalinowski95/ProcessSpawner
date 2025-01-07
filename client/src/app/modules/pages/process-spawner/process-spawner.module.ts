import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { ProcessSpawnerRoutingModule } from './process-spawner-routing.module';
import { ProcessListComponent } from './components/process-list/process-list.component';
import { HttpClientModule } from '@angular/common/http';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { MatButtonModule } from '@angular/material/button';
import { MatCardModule } from '@angular/material/card';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatIconModule } from '@angular/material/icon';
import { MatInputModule } from '@angular/material/input';
import { ElementListModule } from "../../shared/element-list/element-list.module";
import { DynamicFormModule } from '../../shared/dynamic-form/dynamic-form.module';
import { DialogModule } from '../../shared/dialog/dialog.module';
import { MatOptionModule } from '@angular/material/core';
import { MatSelectModule } from '@angular/material/select';
import { MatCheckboxModule } from '@angular/material/checkbox';
import { ProcessInstanceCreateComponent } from './components/process-instance-create/process-instance-create.component';

@NgModule({
  declarations: [
    ProcessListComponent,
    ProcessInstanceCreateComponent
  ],
  imports: [
    CommonModule,
    ProcessSpawnerRoutingModule,
    FormsModule,
    ReactiveFormsModule,
    HttpClientModule,
    MatInputModule,
    MatFormFieldModule,
    MatButtonModule,
    MatCardModule,
    MatIconModule,
    MatOptionModule,
    MatFormFieldModule,
    MatSelectModule,
    MatCheckboxModule,
    ElementListModule,
    DialogModule,
    DynamicFormModule
  ]
})
export class ProcessSpawnerModule { }
