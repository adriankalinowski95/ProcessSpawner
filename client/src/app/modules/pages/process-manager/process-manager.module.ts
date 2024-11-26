import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';

import { HttpClientModule } from '@angular/common/http';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { MatButtonModule } from '@angular/material/button';
import { MatCardModule } from '@angular/material/card';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatIconModule } from '@angular/material/icon';
import { MatInputModule } from '@angular/material/input';
import { ElementListModule } from "../../shared/element-list/element-list.module";
import { ProcessManagerRoutingModule } from './process-manager-routing.module';
import { ProcessManagerListComponent } from './components/process-manager-list/process-manager-list.component';
import { DialogModule } from '@angular/cdk/dialog';
import { DynamicFormModule } from '../../shared/dynamic-form/dynamic-form.module';

@NgModule({
  declarations: [
    ProcessManagerListComponent
  ],
  imports: [
    CommonModule,
    ProcessManagerRoutingModule,
    FormsModule,
    ReactiveFormsModule,
    HttpClientModule,
    MatInputModule,
    MatFormFieldModule,
    MatButtonModule,
    MatCardModule,
    MatIconModule,
    ElementListModule,
    DialogModule,
    DynamicFormModule
  ]
})
export class ProcessManagerModule { }
