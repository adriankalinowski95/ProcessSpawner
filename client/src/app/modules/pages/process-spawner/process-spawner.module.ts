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

@NgModule({
  declarations: [
    ProcessListComponent
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
    ElementListModule
  ]
})
export class ProcessSpawnerModule { }
