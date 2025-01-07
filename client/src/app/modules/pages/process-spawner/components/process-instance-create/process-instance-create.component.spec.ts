import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ProcessInstanceCreateComponent } from './process-instance-create.component';

describe('ProcessInstanceCreateComponent', () => {
  let component: ProcessInstanceCreateComponent;
  let fixture: ComponentFixture<ProcessInstanceCreateComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ProcessInstanceCreateComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(ProcessInstanceCreateComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
